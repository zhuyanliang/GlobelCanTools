#ifndef HEADER_H
#define HEADER_H

//Commands
#define WR_END  0x01 //
#define WR_MEM  0x02 //写程序存储器
#define ER_MEM  0x03 //擦除程序存储器
#define RUN_APP 0x04

#define GUI             ((unsigned char)0xFD)
#define BMS             ((unsigned char)0xF4)

//Function
#define CAN_MSG_IMAGE_ERASE     ((unsigned char)0xF0)
#define CAN_MSG_IMAGE_REQUEST   ((unsigned char)0xF1) //请求App下载数据
#define CAN_MSG_IMAGE_LOAD      ((unsigned char)0xF2)
#define CAN_MSG_OVER_LOAD       ((unsigned char)0xF3) // 下载数据结束标志
#define CAN_MSG_REQ_NEXT        ((unsigned char)0xF4) // 请求下一帧数据
#define CAN_MSG_REQ_AGAIN       ((unsigned char)0xF5) // 请求重新发送该帧数据

//接收到广播数据的标示符
#define BRO_SYS_WARN        0x180150F4  //电压、总电流、故障报警状态等
#define BRO_BATT_INFO       0x180250F4
#define BRO_CELLV_INFO      0x180350F4
#define BRO_CELLT_INFO      0x180450F4
#define BRO_CELL_VOLT1      0x181050F4
#define BRO_CELL_VOLT2      0x181150F4
#define BRO_CELL_VOLT3      0x181250F4
#define BRO_CELL_VOLT4      0x181350F4
#define BRO_CELL_VOLT5      0x181450F4
#define BRO_TEMP_01         0x182050F4

// GUI发送数据请求的标示符
#define REQ_BMS_VERSION     0x1800F4FD
#define REQ_BATT_STAT       0x1801F4FD
#define REQ_CELL_STAT       0x1802F4FD
#define REQ_PACK_SOH        0x180CF4FD
#define REQ_PACK_WARN       0x180DF4FD
#define REQ_PACK_PRA        0x1820F4FD
#define REQ_READ_COC        0x1822F4FD
#define REQ_READ_DOC        0x1823F4FD
#define REQ_READ_COT        0x1824F4FD
#define REQ_READ_DOT        0x1825F4FD
#define REQ_READ_CUT        0x1826F4FD
#define REQ_READ_DUT        0x1827F4FD
#define REQ_READ_OUV        0x1828F4FD  // 电芯的过、欠压参数
#define REQ_READ_DLV        0x1829F4FD  // 单体一致性
#define REQ_READ_DLT        0x182AF4FD  // 温差
#define REQ_READ_POV        0x182BF4FD
#define REQ_READ_PUV        0x182CF4FD

#define REQ_READ_NOR        0x1840F4FD // SOH 三次错误记录
#define REQ_FALT_1          0x1841F4FD // 发生错误次数1
#define REQ_FALT_2          0x1842F4FD // 发生错误次数2
#define REQ_FALT_3          0x1843F4FD // 发生错误次数3
#define REQ_FALT_4          0x1844F4FD // 发生错误次数4
#define REQ_FALT_5          0x1846F4FD // 发生错误次数5

// GUI接收到所请求数据的标示符
#define REC_BMS_VERSION     0x1800FDF4
#define REC_BATT_STAT       0x1801FDF4
#define REC_CELL_STAT       0x1802FDF4
#define REC_PACK_SOH        0x180CFDF4
#define REC_PACK_WARN       0x180DFDF4
#define REC_PACK_PRA        0x1820FDF4
#define REC_READ_COC        0x1822FDF4
#define REC_READ_DOC        0x1823FDF4
#define REC_READ_COT        0x1824FDF4
#define REC_READ_DOT        0x1825FDF4
#define REC_READ_CUT        0x1826FDF4
#define REC_READ_DUT        0x1827FDF4
#define REC_READ_OUV        0x1828FDF4
#define REC_READ_DLV        0x1829FDF4  // 单体一致性
#define REC_READ_DLT        0x182AFDF4  // 温差
#define REC_READ_POV        0x182BFDF4
#define REC_READ_PUV        0x182CFDF4

#define REC_READ_NOR        0x1840FDF4 // SOH 三次错误记录
#define REC_FALT_1          0x1841FDF4 // 发生错误次数1
#define REC_FALT_2          0x1842FDF4 // 发生错误次数2
#define REC_FALT_3          0x1843FDF4 // 发生错误次数3
#define REC_FALT_4          0x1844FDF4 // 发生错误次数4
#define REC_FALT_5          0x1846FDF4 // 发生错误次数5

// GUI读写内部EEPROM
#define REQ_WRITE_EEPROM        0x1830F4FD
#define REQ_READ_EEPROM         0x1831F4FD

// 上位机配置BMS部分参数
#define  CAN_GUI_CONFIG_COV_TH                 ((uint8_t)0x10)
#define  CAN_GUI_CONFIG_CUV_TH                 ((uint8_t)0x11)
#define  CAN_GUI_CONFIG_COT_TH                 ((uint8_t)0x12)
#define  CAN_GUI_CONFIG_CUT_TH                 ((uint8_t)0x13)
#define  CAN_GUI_CONFIG_DOT_TH                 ((uint8_t)0x14)
#define  CAN_GUI_CONFIG_DUT_TH                 ((uint8_t)0x15)
#define  CAN_GUI_CONFIG_COC_TH                 ((uint8_t)0x16)
#define  CAN_GUI_CONFIG_DOC_TH                 ((uint8_t)0x17)
#define  CAN_GUI_CONFIG_DLV_TH                 ((uint8_t)0x18)
#define  CAN_GUI_CONFIG_DLT_TH                 ((uint8_t)0x19)
#define  CAN_GUI_CONFIG_POV_TH                 ((uint8_t)0x1A)
#define  CAN_GUI_CONFIG_PUV_TH                 ((uint8_t)0x1B)
#define  CAN_GUI_CONFIG_ISO_TH                 ((uint8_t)0x1C)
#define  CAN_GUI_CONFIG_PDIM_TH                ((uint8_t)0x1D)


// 错误信息代码
#define FALT_CUV 	0xE0 // 电芯低压
#define FALT_COV 	0xE1 // 电芯高压
#define FALT_CUT	0xE2 // 充电低温
#define FALT_COT	0xE3 // 充电高温
#define FALT_DUT	0xE4 // 放电低温
#define FALT_DOT	0xE5 // 放电高温
#define FALT_COC	0xE6 // 充电过流
#define FALT_DOC	0xE7 // 放电过流
#define FALT_VDIF	0xE8 // 电芯电压一致性
#define FALT_TDIF	0xE9 // 温差
#define FALT_POV 	0xEA // 包过压
#define FALT_PUV	0xEB // 包低压
#define FALT_LTC	0xEC // 6803错误


#endif // HEADER_H
