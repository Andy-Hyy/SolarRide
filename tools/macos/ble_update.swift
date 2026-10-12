import Foundation
import CoreBluetooth
import CryptoKit
import AppKit
import UniformTypeIdentifiers

func crc16(_ data:Data)->UInt16 {
    var c:UInt16=0
    for b in data {c ^= UInt16(b)<<8;for _ in 0..<8 {c = c&0x8000 != 0 ? (c &<< 1)^0x1021 : c &<< 1}}
    return c
}
func le16(_ n:UInt16)->Data {Data([UInt8(truncatingIfNeeded:n),UInt8(truncatingIfNeeded:n>>8)])}
func read16(_ d:Data,_ i:Int)->UInt16 {UInt16(d[i])|(UInt16(d[i+1])<<8)}
func command(_ id:UInt16,_ size:Int=0)->Data {
    var p=le16(id);for i in 0..<4 {p.append(UInt8(truncatingIfNeeded:size>>(8*i)))}
    p.append(Data(repeating:0,count:18-p.count));p.append(le16(crc16(p)));return p
}
func hex(_ d:Data)->String {d.map{String(format:"%02x",$0)}.joined()}
func validateImage(_ data:Data)throws->String {
    guard data.count>=288,data.count<=0x140000,data[0]==0xe9,read16(data,12)==5,
        data[23]==1,Array(data[32..<36])==[0x32,0x54,0xcd,0xab] else {
        throw NSError(domain:"SolarRider",code:1,userInfo:[NSLocalizedDescriptionKey:"请选择 ESP32-C3 的应用固件 .bin，不能使用 merged 或 bootloader 文件。"])
    }
    guard Data(SHA256.hash(data:Data(data.dropLast(32))))==Data(data.suffix(32)) else {
        throw NSError(domain:"SolarRider",code:2,userInfo:[NSLocalizedDescriptionKey:"固件完整性校验失败。"])
    }
    guard data.range(of:Data("SolarRider_OTA_PROTOCOL_1".utf8)) != nil else {
        throw NSError(domain:"SolarRider",code:3,userInfo:[NSLocalizedDescriptionKey:"此固件缺少后续蓝牙更新功能，请使用配套升级文件。"])
    }
    return hex(Data(data[176..<208]))
}
func sectorPackets(_ data:Data,index:Int,mtu:Int)->[Data] {
    let step=mtu-5;precondition(step>=17)
    let count=(data.count+step-1)/step;precondition(count<=255)
    let crc=crc16(data);var result:[Data]=[]
    for i in 0..<count {
        let start=i*step,end=min(start+step,data.count),tail=i==count-1
        var p=le16(UInt16(index));p.append(tail ? 0xff : UInt8(i));p.append(data[start..<end])
        if tail {p.append(le16(crc))};result.append(p)
    };return result
}
if CommandLine.arguments.contains("--self-test") {
    precondition(crc16(Data("123456789".utf8))==0x31c3)
    let p=command(1,123456);precondition(p.count==20 && read16(p,18)==crc16(Data(p.prefix(18))))
    let src=Data((0..<4096).map{UInt8(truncatingIfNeeded:$0)})
    for mtu in [22,182,244] {
        let packets=sectorPackets(src,index:7,mtu:mtu)
        precondition(packets.allSatisfy{$0.count<=mtu} && packets.last![2]==0xff)
        var restored=Data()
        for p in packets {restored.append(p[3..<(p.count-(p[2]==0xff ? 2:0))])}
        precondition(restored==src && read16(packets.last!,packets.last!.count-2)==crc16(src))
    }
    print("PASS: CRC reference vector, official command framing, sector reconstruction at ATT payload 22/182/244")
    exit(0)
}
if CommandLine.arguments.contains("--validate"),let path=CommandLine.arguments.last {
    do {let d=try Data(contentsOf:URL(fileURLWithPath:path));print("PASS: image integrity, C3 identity, OTA marker; ELF SHA256=\(try validateImage(d))")}
    catch {fputs("\(error.localizedDescription)\n",stderr);exit(1)}
    exit(0)
}

final class Uploader:NSObject,CBCentralManagerDelegate,CBPeripheralDelegate {
    let service=CBUUID(string:"8018"),fwUUID=CBUUID(string:"8020"),cmdUUID=CBUUID(string:"8022"),infoUUID=CBUUID(string:"8023")
    var central:CBCentralManager!,peripheral:CBPeripheral?
    var firmwareChar:CBCharacteristic?,commandChar:CBCharacteristic?,infoChar:CBCharacteristic?
    let image:Data,expectedELF:String
    var phase="scan",done=false,result=1,started=Date(),lastEvent=Date(),lastRead=Date.distantPast
    var sector=0,packets:[Data]=[],packetIndex=0,lastWriteIsTail=false,tailWriteDone=false,sectorAck=false
    var startWriteDone=false,startAccepted=false,commitSent=false
    init(image:Data,expected:String){self.image=image;expectedELF=expected;super.init();central=CBCentralManager(delegate:self,queue:nil)}
    func log(_ s:String){print(s);fflush(stdout)}
    func fail(_ s:String){guard !done else{return};log("更新未确认：\(s)");done=true;central.stopScan();if let p=peripheral{central.cancelPeripheralConnection(p)}}
    func centralManagerDidUpdateState(_ c:CBCentralManager){
        if c.state == .poweredOn {log("正在寻找附近的 SolarRider…");c.scanForPeripherals(withServices:[service],options:nil)}
        else if c.state == .unauthorized {fail("请在系统设置中允许此工具使用蓝牙。")}
        else if c.state == .poweredOff {fail("请打开 Mac 蓝牙。")}
    }
    func centralManager(_ c:CBCentralManager,didDiscover p:CBPeripheral,advertisementData:[String:Any],rssi:NSNumber){
        let name=advertisementData[CBAdvertisementDataLocalNameKey] as? String ?? p.name ?? ""
        guard name=="SolarRider",phase=="scan" else{return}
        peripheral=p;p.delegate=self;phase="connect";lastEvent=Date();c.stopScan();c.connect(p,options:nil)
    }
    func centralManager(_ c:CBCentralManager,didConnect p:CBPeripheral){
        lastEvent=Date();phase=commitSent ? "verify" : "subscribe";p.discoverServices([service])
        log(commitSent ? "已重新连接，正在检查新程序…" : "已连接。首次升级如系统提示，请输入配对码文件中的六位数字。")
    }
    func centralManager(_ c:CBCentralManager,didFailToConnect p:CBPeripheral,error:Error?){fail(error?.localizedDescription ?? "连接失败。")}
    func centralManager(_ c:CBCentralManager,didDisconnectPeripheral p:CBPeripheral,error:Error?){
        if done{return}
        if commitSent {
            phase="scan";lastEvent=Date();firmwareChar=nil;commandChar=nil;infoChar=nil
            c.scanForPeripherals(withServices:[service],options:nil)
        }else{fail("传输期间连接断开。新固件尚未确认；可重新启动工具重传。")}
    }
    func peripheral(_ p:CBPeripheral,didDiscoverServices error:Error?){
        if let e=error{fail(e.localizedDescription);return}
        for s in p.services ?? [] where s.uuid==service {p.discoverCharacteristics([fwUUID,cmdUUID,infoUUID],for:s)}
    }
    func peripheral(_ p:CBPeripheral,didDiscoverCharacteristicsFor s:CBService,error:Error?){
        if let e=error{fail(e.localizedDescription);return}
        for ch in s.characteristics ?? [] {
            if ch.uuid==fwUUID{firmwareChar=ch};if ch.uuid==cmdUUID{commandChar=ch};if ch.uuid==infoUUID{infoChar=ch}
        }
        if commitSent {if let ch=infoChar{p.readValue(for:ch);lastRead=Date()}else{fail("新程序没有版本检查接口。")};return}
        guard let f=firmwareChar,let c=commandChar else{fail("未发现升级服务，请先通过 USB 安装 V6。");return}
        let mtu=min(244,p.maximumWriteValueLength(for:.withoutResponse))
        guard mtu>=22 else{fail("蓝牙包容量过小，无法使用该升级协议。请重新连接。");return}
        p.setNotifyValue(true,for:f);p.setNotifyValue(true,for:c)
    }
    func peripheral(_ p:CBPeripheral,didUpdateNotificationStateFor ch:CBCharacteristic,error:Error?){
        if let e=error{fail(e.localizedDescription);return}
        if phase=="subscribe",firmwareChar?.isNotifying==true,commandChar?.isNotifying==true {
            phase="start";lastEvent=Date();p.writeValue(command(1,image.count),for:commandChar!,type:.withResponse)
        }
    }
    func beginSector(){
        guard let p=peripheral,firmwareChar != nil else{return}
        let begin=sector*4096
        if begin>=image.count {
            commitSent=true;phase="commit";lastEvent=Date();p.writeValue(command(2),for:commandChar!,type:.withResponse);return
        }
        packets=sectorPackets(Data(image[begin..<min(begin+4096,image.count)]),index:sector,mtu:min(244,p.maximumWriteValueLength(for:.withoutResponse)))
        packetIndex=0;tailWriteDone=false;sectorAck=false;phase="sector";sendPacket()
    }
    func sendPacket(){
        guard phase=="sector",packetIndex<packets.count,let p=peripheral,let ch=firmwareChar else{return}
        let data=packets[packetIndex];packetIndex+=1;lastWriteIsTail=data[2]==0xff
        lastEvent=Date();p.writeValue(data,for:ch,type:.withResponse)
    }
    func advanceSectorIfReady(){
        if phase=="sector" && tailWriteDone && sectorAck {
            sector+=1;log("上传进度 \(min(100,sector*4096*100/image.count))%");beginSector()
        }
    }
    func peripheral(_ p:CBPeripheral,didWriteValueFor ch:CBCharacteristic,error:Error?){
        if let e=error{fail(e.localizedDescription);return};lastEvent=Date()
        if ch.uuid==cmdUUID,phase=="start" {startWriteDone=true;if startAccepted{beginSector()}}
        else if ch.uuid==fwUUID,phase=="sector" {
            if lastWriteIsTail {tailWriteDone=true;advanceSectorIfReady()}else{sendPacket()}
        }
    }
    func peripheral(_ p:CBPeripheral,didUpdateValueFor ch:CBCharacteristic,error:Error?){
        if let e=error{fail(e.localizedDescription);return};guard let data=ch.value else{return};lastEvent=Date()
        if ch.uuid==infoUUID && commitSent {
            guard let info=try? JSONSerialization.jsonObject(with:data) as? [String:String] else{fail("版本检查数据无效。");return}
            guard info["elf_sha256"]==expectedELF else{fail("设备运行的固件与升级文件不一致，可能已退回旧版。");return}
            if info["boot"]=="confirmed" {
                log("更新完成：新程序已重启，版本身份和启动检查均通过。")
                result=0;done=true;central.cancelPeripheralConnection(p)
            }else{phase="health";lastRead=Date()};return
        }
        guard data.count==20,read16(data,18)==crc16(Data(data.prefix(18))) else{fail("升级确认包校验失败。");return}
        if ch.uuid==cmdUUID,read16(data,0)==3 {
            let id=read16(data,2),status=read16(data,4)
            if id==1 {
                if status != 0{fail("车包拒绝开始升级。请停车静止，开机等待约 8 秒后重试。");return}
                startAccepted=true;if startWriteDone{beginSector()}
            }else if id==2 {
                if status != 0{fail("固件未通过最终检查，车包保留旧程序。");return}
                phase="reboot";log("文件已完整写入并校验，等待车包重启…")
            }
        }else if ch.uuid==fwUUID {
            guard phase=="sector",Int(read16(data,0))==sector,read16(data,2)==0 else{fail("车包拒绝数据块，升级中止。");return}
            sectorAck=true;advanceSectorIfReady()
        }
    }
    func tick(){
        if done{return}
        if Date().timeIntervalSince(started)>900 || Date().timeIntervalSince(lastEvent)>60 {fail("等待超时。车包可能不在附近，或尚未安装无线更新版本。");return}
        if phase=="health",Date().timeIntervalSince(lastRead)>1,let p=peripheral,let ch=infoChar{lastRead=Date();p.readValue(for:ch)}
    }
}
var selectedPath=CommandLine.arguments.dropFirst().first
if selectedPath==nil {
    let app=NSApplication.shared;app.setActivationPolicy(.accessory);app.activate(ignoringOtherApps:true)
    let panel=NSOpenPanel();panel.title="选择 SolarRider 蓝牙升级固件";panel.allowedContentTypes=[UTType(filenameExtension:"bin") ?? .data];panel.allowsMultipleSelection=false
    guard panel.runModal() == .OK,let url=panel.url else{exit(0)};selectedPath=url.path
}
do {
    let image=try Data(contentsOf:URL(fileURLWithPath:selectedPath!));let elf=try validateImage(image)
    print("文件检查通过。请让车包停车静止、保持供电，并关闭其他已连接它的蓝牙工具。")
    let uploader=Uploader(image:image,expected:elf)
    while !uploader.done {RunLoop.main.run(until:Date().addingTimeInterval(0.1));uploader.tick()}
    exit(Int32(uploader.result))
}catch{fputs("\(error.localizedDescription)\n",stderr);exit(1)}
