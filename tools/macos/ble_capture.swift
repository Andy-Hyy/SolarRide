import Foundation
import CoreBluetooth

// Local BLE telemetry client. No pairing database or system settings changes.
final class Capture: NSObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    let service = CBUUID(string: "6E400001-B5A3-F393-E0A9-E50E24DCCA9E")
    let tx = CBUUID(string: "6E400003-B5A3-F393-E0A9-E50E24DCCA9E")
    var central: CBCentralManager!
    var peripheral: CBPeripheral?
    var data = Data()
    var received = 0
    var started = Date()
    var deadline: TimeInterval
    var subscribed = false
    var finishing = false
    init(seconds: TimeInterval) {
        deadline = seconds
        super.init()
        central = CBCentralManager(delegate: self, queue: nil)
    }
    func log(_ text: String) { print("#BLE,\(text)"); fflush(stdout) }
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        log("state=\(central.state.rawValue),authorization=\(CBCentralManager.authorization.rawValue)")
        if central.state == .poweredOn {
            central.scanForPeripherals(withServices: [service], options: nil)
            log("scanning_for=SolarRider")
        } else if central.state == .unauthorized { log("ERROR,macOS_bluetooth_permission_required") }
    }
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                        advertisementData: [String: Any], rssi RSSI: NSNumber) {
        let name = advertisementData[CBAdvertisementDataLocalNameKey] as? String ?? peripheral.name ?? ""
        log("found,name=\(name),id=\(peripheral.identifier.uuidString),rssi=\(RSSI)")
        guard name == "SolarRider" else { return }
        guard self.peripheral?.state != .connecting && self.peripheral?.state != .connected else { return }
        self.peripheral = peripheral
        peripheral.delegate = self
        central.stopScan()
        central.connect(peripheral, options: nil)
    }
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        data.removeAll()
        log("connected,name=\(peripheral.name ?? "")")
        peripheral.discoverServices([service])
    }
    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        log("connect_failed,\(String(describing:error))")
    }
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        subscribed = false
        log("disconnected,\(String(describing:error))")
        if !finishing && central.state == .poweredOn {
            data.removeAll()
            central.scanForPeripherals(withServices:[service],options:nil)
            log("scanning_after_disconnect")
        }
    }
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard error == nil else { log("service_error,\(error!)"); return }
        for s in peripheral.services ?? [] where s.uuid == service {
            log("service=\(s.uuid)");peripheral.discoverCharacteristics([tx], for: s)
        }
    }
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        guard error == nil else { log("characteristic_error,\(error!)"); return }
        for c in service.characteristics ?? [] where c.uuid == tx {
            log("subscribing,tx=\(c.uuid)");peripheral.setNotifyValue(true, for: c)
        }
    }
    func peripheral(_ peripheral: CBPeripheral, didUpdateNotificationStateFor characteristic: CBCharacteristic, error: Error?) {
        subscribed = characteristic.isNotifying
        log("notifications=\(subscribed),error=\(String(describing:error))")
    }
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        guard error == nil, let bytes = characteristic.value else { return }
        data.append(bytes)
        while let end = data.firstIndex(of: 10) {
            let line = data.prefix(upTo: end)
            let text = String(decoding:line,as:UTF8.self)
            let fields = text.split(separator:",",omittingEmptySubsequences:false)
            // Subscription can begin in the middle of a fragmented CSV row.
            if (fields.count == 22 || fields.count == 26) && fields.allSatisfy({ Double($0) != nil }) {
                print(text);fflush(stdout);received += 1
            } else { log("skip_incomplete_csv_row") }
            data.removeSubrange(...end)
        }
        if data.count > 2048 { data.removeAll(); log("ERROR,incomplete_line_overflow") }
    }
    func finish() {
        finishing = true
        central.stopScan()
        if let p=peripheral, p.state == .connected { central.cancelPeripheralConnection(p) }
        log("summary,subscribed=\(subscribed),complete_rows=\(received)")
    }
}

let seconds = CommandLine.arguments.count>1 ? Double(CommandLine.arguments[1]) ?? 60 : 60
let capture = Capture(seconds:seconds)
let limit = Date().addingTimeInterval(seconds)
while Date()<limit { RunLoop.main.run(until:Date().addingTimeInterval(0.1)) }
capture.finish()
RunLoop.main.run(until:Date().addingTimeInterval(0.5))
