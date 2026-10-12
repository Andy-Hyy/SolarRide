# PCB sources and fabrication

[English](#english) · [中文](#中文)

## English

`SolarRider_Board_20261012.epro2` is the supplied EasyEDA Pro project. It includes two PCB documents; the older carrier with two 16-position ESP32 headers matches the existing photographed build. The associated PCB1 export is included as [PCB1_20260626_Gerbers.zip](fabrication/PCB1_20260626_Gerbers.zip).

The ZIP contains the original 13 Gerber layers and two drill files from the June 2026 design export, plus a short bilingual readme. Board outline, top silkscreen and PTH/NPTH drill files were byte-matched against the supplied manufacturing input package. Personal manufacturing metadata and the generated flying-probe input JSON are not included. A file called FlyingProbeTesting is not treated as evidence of a passed physical board test.

Inspect the design before ordering. This package does not establish a new DRC pass, electrical-rule pass, fabricator approval, board thickness, material, copper weight or suitability of substituted modules. Confirm those fabrication settings separately. Module wiring follows the [power guide](../../docs/en/power-wiring.md), including the 4056-TP output to XR2981 input.

Original hardware design: CERN-OHL-P-2.0, Andy-Hyy, 2026. Documentation: CC BY 4.0. Vendor/library assets retain their own terms.

## 中文

**PCB 源文件与生产文件**

EasyEDA Pro 工程含两个 PCB 文档，每侧 16 针 ESP32 排针的较早载板与已有组装照片相符。相应 PCB1 导出见 [PCB1_20260626_Gerbers.zip](fabrication/PCB1_20260626_Gerbers.zip)。

压缩包保留 2026 年 6 月导出的 13 个 Gerber 层和两个钻孔文件，另附双语说明。板框、顶层丝印及通孔/非金属化孔钻孔已与所供厂家输入包逐字节核对。不包含订单元数据和飞针输入 JSON，也不把 FlyingProbeTesting 文件当作实物测试通过证据。

下单前应核对设计。本包不声明新增 DRC/ERC 通过、厂家批准、板厚、材料、铜厚或替换模块适配，制造参数需另行确认。模块接线见[供电指南](../../docs/zh-CN/power-wiring.md)，其中包含 4056-TP 输出到 XR2981 输入。

原创硬件采用 CERN-OHL-P-2.0，署名 Andy-Hyy，2026；说明文档采用 CC BY 4.0，厂商与库资源保留原许可。
