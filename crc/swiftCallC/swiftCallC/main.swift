//
//  main.swift
//  swiftCallC
//
//  Created by 丁松 on 14-9-4.
//  Copyright (c) 2014年 丁松. All rights reserved.
//

import Foundation

println("Hello, World!")

var dataBuffer = "Hello CRC!"

var crc8 : UInt8 = 0
init_crc8_table();
crc8Init(&crc8);
crc8Update(&crc8, dataBuffer, countElements(dataBuffer));

println("crc8 = \(crc8)")
