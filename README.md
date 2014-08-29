DDT
===
Digital Data Transmition,数字数据传输,加密,通信协议,通信接口,USART,IIC,JTAG,USB,CRC,HASH


##CRC32
有C和汇编码两个，是用查表法实现，需要256个32bit空间存储，另外需要几个byte空间用中间量。
汇编码需要用到移位、加法、异或、与、或、指令，256个32bitsram,8个32bit栈空间，一个数据流输入，总共需要3~4个寄存器，其中至少一个能够参与各个指令的运算，至少一个能用来寻址sram。

##CRC16
Refer [HezhouW](https://github.com/HezhouW/CRC16_CCITT), [htmlxx](http://blog.csdn.net/htmlxx/article/details/17369105#comments),
