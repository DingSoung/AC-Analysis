//
//  main.swift
//  swiftCallC
//
//  Created by 丁松 on 14-9-4.
//  Copyright (c) 2014年 丁松. All rights reserved.
//

import Foundation

println("Hello, World!")

var dataBuffer:[UInt8] = [0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x43, 0x52, 0x43, 0x21]

var crc8 : UInt8 = 0
init_crc8_table();
crc8Init(&crc8);
crc8Update(&crc8, &dataBuffer, (UInt32)(dataBuffer.count));
crc8Finish(&crc8);

println("crc8 = \(crc8)")
