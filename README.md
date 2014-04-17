electricity_parameters
======================

electricity parameters such as rfft frequenct  iirlpf rms frequency power


说明
====
	我这里重点考虑性能和通用、可移植性，少量依赖编译器优化，较少依赖通用硬件单元，不依赖特殊平台。



傅里叶展开
==========
	对于测量系统，输入一般都是一维实数
	这里FFT运算是将对buffer（实数）进行分析，得到每个频率及对应的幅度.
	原理是将1个N点的时域信号分成N个1点的时域信号，然后计算这N个1点时域信号的频域，得到N个频域的点，然后将这个N个频域的点按照一定的顺序加起来，就得到了需要的频谱。
	标准正弦信号x(t) = Acos(ω。t＋φ) + B =  Acos(2πf*t＋φ) + B,模拟多个不同频率 相位 幅度的信号叠加

clear all;
fs = 4000;              % 采样频率
f1 = 50;    f2 = 120;   % 信号频率
A1 = 220;   A2 = 380;   % 信号幅度
DC = 72;                % 直流
n = fs*10;              % 总的采样点数
t = (0:n-1)*(1/fs);     % 时间序列(时间轴)
x =  A1*sqrt(2)*cos(2*pi*f1*t) + A2*sqrt(2)*cos(2*pi*f2*t);   %产生信号
x = x + 300*randn(size(t));		% 混入噪声信号
subplot(311)
plot(t(1:1000),x(1:1000))		%显示前1000个点
xlabel('time (S)')
subplot(312)
plot(t(1:200)*1000,x(1:200))	%前200个点
xlabel('time (mS)')
N = 2^nextpow2(n);      % 求得最接近总采样点的2^n,也可以是指定的长度
X = fft(x,N)/n;         %进行fft变换（除以总采样点数，是为了后面精确看出原始信号幅值）  
F = fs/2*(0,N/2);%linspace(0,fs/2,N/2+1);	%频率轴(只画到Fs/2即可，由于y为实数，后面一半是对称的)
subplot(313)			% 画出频率幅度图形
plot(F,2*abs(X(1:N/2+1)))
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')

FFTW库
------
	大致是先用fftw_malloc分配输入输出内存，然后输入数据赋值，然后创建变换方案（fftw_plan），然后执行变换（fftw_execute），最后释放资源，还是比较简单的
	使用FFTW库的函数 fftw_plan_dft_r2c_1d()，另逆变换为fftw_plan_dft_c2r_1d()


	还可以针对特殊条件进行优化：固定分析的长度，如1024，将sin cos改成查表法等。再就是利用硬件优势了，如使用汇编，操作寄存器使用硬件DSP，这里不考虑。



数字滤波器
=========
	I2R数字滤波器
	包括输入和输出流，参考标准算法，使用Matlab FDAtool生成滤波器参数
	没有浮点单元的，使用增益进行整形运算

% IIR滤波器设计  
% 目的：设计一个采样频率为1000Hz、通带截止频率为50Hz、阻带截止频率为100Hz的低通滤波器，并要求通带最大衰减为1dB，阻带最小衰减为60dB。  
  
clc;clear;close all;  
  
% 1. 产生信号（频率为10Hz和100Hz的正弦波叠加）
Fs=1000; % 采样频率1000Hz
t=0:1/Fs:1;
s10=sin(20*pi*t); % 产生10Hz正弦波
s100=sin(200*pi*t); % 产生100Hz正弦波
s=s10+s100; % 信号叠加

figure(1); % 画图
subplot(2,1,1);plot(s);grid;
title('原始信号');

% 2. FFT分析信号频谱
len = 512;
y=fft(s,len);  % 对信号做len点FFT变换
f=Fs*(0:len/2 - 1)/len;

subplot(2,1,2);plot(f,abs(y(1:len/2)));grid;
title('原始信号频谱')
xlabel('Hz');ylabel('幅值');
  
% 3. IIR滤波器设计  
N=0; % 阶数  
Fp=50; % 通带截止频率50Hz  
Fc=100; % 阻带截止频率100Hz  
Rp=1; % 通带波纹最大衰减为1dB  
Rs=60; % 阻带衰减为60dB  
  
% 3.0 计算最小滤波器阶数  
na=sqrt(10^(0.1*Rp)-1);  
ea=sqrt(10^(0.1*Rs)-1);  
N=ceil(log10(ea/na)/log10(Fc/Fp));  
  
% 3.1 巴特沃斯滤波器  
Wn=Fp*2/Fs;  
[Bb Ba]=butter(N,Wn,'low'); % 调用MATLAB butter函数快速设计滤波器  
[BH,BW]=freqz(Bb,Ba); % 绘制频率响应曲线  
Bf=filter(Bb,Ba,s); % 进行低通滤波  
By=fft(Bf,len);  % 对信号f1做len点FFT变换  
  
figure(2); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,Bf,'red');grid;  
legend('10Hz原始信号','巴特沃斯滤波器滤波后');  
subplot(2,1,2);plot(f,abs(By(1:len/2)));grid;  
title('巴特沃斯低通滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 3.2 切比雪夫I型滤波器  
[C1b C1a]=cheby1(N,Rp,Wn,'low'); % 调用MATLAB cheby1函数快速设计低通滤波器  
[C1H,C1W]=freqz(C1b,C1a); % 绘制频率响应曲线  
C1f=filter(C1b,C1a,s); % 进行低通滤波  
C1y=fft(C1f,len);  % 对滤波后信号做len点FFT变换  
  
figure(3); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,C1f,'red');grid;  
legend('10Hz原始信号','切比雪夫I型滤波器滤波后');  
subplot(2,1,2);plot(f,abs(C1y(1:len/2)));grid;  
title('切比雪夫I型滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 3.3 切比雪夫II型滤波器  
[C2b C2a]=cheby2(N,Rs,Wn,'low'); % 调用MATLAB cheby2函数快速设计低通滤波器  
[C2H,C2W]=freqz(C2b,C2a); % 绘制频率响应曲线  
C2f=filter(C2b,C2a,s); % 进行低通滤波  
C2y=fft(C2f,len);  % 对滤波后信号做len点FFT变换  
  
figure(4); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,C2f,'red');grid;  
legend('10Hz原始信号','切比雪夫II型滤波器滤波后');  
subplot(2,1,2);plot(f,abs(C2y(1:len/2)));grid;  
title('切比雪夫II型滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 3.4 椭圆滤波器  
[Eb Ea]=ellip(N,Rp,Rs,Wn,'low'); % 调用MATLAB ellip函数快速设计低通滤波器  
[EH,EW]=freqz(Eb,Ea); % 绘制频率响应曲线  
Ef=filter(Eb,Ea,s); % 进行低通滤波  
Ey=fft(Ef,len);  % 对滤波后信号做len点FFT变换  
  
figure(5); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,Ef,'red');grid;  
legend('10Hz原始信号','椭圆滤波器滤波后');  
subplot(2,1,2);plot(f,abs(Ey(1:len/2)));grid;  
title('椭圆滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 3.5 yulewalk滤波器  
fyule=[0 Wn Fc*2/Fs 1]; % 在此进行的是简单设计，实际需要多次仿真取最佳值  
myule=[1 1 0 0]; % 在此进行的是简单设计，实际需要多次仿真取最佳值  
[Yb Ya]=yulewalk(N,fyule,myule); % 调用MATLAB yulewalk函数快速设计低通滤波器  
[YH,YW]=freqz(Yb,Ya); % 绘制频率响应曲线  
Yf=filter(Yb,Ya,s); % 进行低通滤波  
Yy=fft(Yf,len);  % 对滤波后信号做len点FFT变换  
  
figure(6); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,Yf,'red');grid;  
legend('10Hz原始信号','yulewalk滤波器滤波后');  
subplot(2,1,2);plot(f,abs(Yy(1:len/2)));grid;  
title('yulewalk滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 4. 各个滤波器的幅频响应对比分析  
A1 = BW*Fs/(2*pi);  
A2 = C1W*Fs/(2*pi);  
A3 = C2W*Fs/(2*pi);  
A4 = EW*Fs/(2*pi);  
A5 = YW*Fs/(2*pi);  
  
figure(7); % 画图  
subplot(1,1,1);plot(A1,abs(BH),A2,abs(C1H),A3,abs(C2H),A4,abs(EH),A5,abs(YH));grid;  
xlabel('频率／Hz');  
ylabel('频率响应幅度');  
legend('butter','cheby1','cheby2','ellip','yulewalk');  










============================================

% IIR滤波器设计  
% 目的：设计一个采样频率为1000Hz、通带截止频率为50Hz、阻带截止频率为100Hz的低通滤波器，并要求通带最大衰减为1dB，阻带最小衰减为60dB。  
  
clc;clear;close all;  
  
% 1. 产生信号（频率为10Hz和100Hz的正弦波叠加）  
Fs=1000; % 采样频率1000Hz  
t=0:1/Fs:1;  
s10=sin(20*pi*t); % 产生10Hz正弦波  
s100=sin(200*pi*t); % 产生100Hz正弦波  
s=s10+s100; % 信号叠加  
  
figure(1); % 画图  
subplot(2,1,1);plot(s);grid;  
title('原始信号');  
  
% 2. FFT分析信号频谱  
len = 512;  
y=fft(s,len);  % 对信号做len点FFT变换  
f=Fs*(0:len/2 - 1)/len;  
  
subplot(2,1,2);plot(f,abs(y(1:len/2)));grid;  
title('原始信号频谱')  
xlabel('Hz');ylabel('幅值');  
  
% 3. IIR滤波器设计  
N=0; % 阶数  
Fp=50; % 通带截止频率50Hz  
Fc=100; % 阻带截止频率100Hz  
Rp=1; % 通带波纹最大衰减为1dB  
Rs=60; % 阻带衰减为60dB  
  
% 3.0 计算最小滤波器阶数  
na=sqrt(10^(0.1*Rp)-1);  
ea=sqrt(10^(0.1*Rs)-1);  
N=ceil(log10(ea/na)/log10(Fc/Fp));  
  
% 3.1 巴特沃斯滤波器  
Wn=Fp*2/Fs;  
[Bb Ba]=butter(N,Wn,'low'); % 调用MATLAB butter函数快速设计滤波器  
[BH,BW]=freqz(Bb,Ba); % 绘制频率响应曲线  
Bf=filter(Bb,Ba,s); % 进行低通滤波  
By=fft(Bf,len);  % 对信号f1做len点FFT变换  
  
figure(2); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,Bf,'red');grid;  
legend('10Hz原始信号','巴特沃斯滤波器滤波后');  
subplot(2,1,2);plot(f,abs(By(1:len/2)));grid;  
title('巴特沃斯低通滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 3.2 切比雪夫I型滤波器  
[C1b C1a]=cheby1(N,Rp,Wn,'low'); % 调用MATLAB cheby1函数快速设计低通滤波器  
[C1H,C1W]=freqz(C1b,C1a); % 绘制频率响应曲线  
C1f=filter(C1b,C1a,s); % 进行低通滤波  
C1y=fft(C1f,len);  % 对滤波后信号做len点FFT变换  
  
figure(3); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,C1f,'red');grid;  
legend('10Hz原始信号','切比雪夫I型滤波器滤波后');  
subplot(2,1,2);plot(f,abs(C1y(1:len/2)));grid;  
title('切比雪夫I型滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 3.3 切比雪夫II型滤波器  
[C2b C2a]=cheby2(N,Rs,Wn,'low'); % 调用MATLAB cheby2函数快速设计低通滤波器  
[C2H,C2W]=freqz(C2b,C2a); % 绘制频率响应曲线  
C2f=filter(C2b,C2a,s); % 进行低通滤波  
C2y=fft(C2f,len);  % 对滤波后信号做len点FFT变换  
  
figure(4); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,C2f,'red');grid;  
legend('10Hz原始信号','切比雪夫II型滤波器滤波后');  
subplot(2,1,2);plot(f,abs(C2y(1:len/2)));grid;  
title('切比雪夫II型滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 3.4 椭圆滤波器  
[Eb Ea]=ellip(N,Rp,Rs,Wn,'low'); % 调用MATLAB ellip函数快速设计低通滤波器  
[EH,EW]=freqz(Eb,Ea); % 绘制频率响应曲线  
Ef=filter(Eb,Ea,s); % 进行低通滤波  
Ey=fft(Ef,len);  % 对滤波后信号做len点FFT变换  
  
figure(5); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,Ef,'red');grid;  
legend('10Hz原始信号','椭圆滤波器滤波后');  
subplot(2,1,2);plot(f,abs(Ey(1:len/2)));grid;  
title('椭圆滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 3.5 yulewalk滤波器  
fyule=[0 Wn Fc*2/Fs 1]; % 在此进行的是简单设计，实际需要多次仿真取最佳值  
myule=[1 1 0 0]; % 在此进行的是简单设计，实际需要多次仿真取最佳值  
[Yb Ya]=yulewalk(N,fyule,myule); % 调用MATLAB yulewalk函数快速设计低通滤波器  
[YH,YW]=freqz(Yb,Ya); % 绘制频率响应曲线  
Yf=filter(Yb,Ya,s); % 进行低通滤波  
Yy=fft(Yf,len);  % 对滤波后信号做len点FFT变换  
  
figure(6); % 画图  
subplot(2,1,1);plot(t,s10,'blue',t,Yf,'red');grid;  
legend('10Hz原始信号','yulewalk滤波器滤波后');  
subplot(2,1,2);plot(f,abs(Yy(1:len/2)));grid;  
title('yulewalk滤波后信号频谱');  
xlabel('Hz');ylabel('幅值');  
  
% 4. 各个滤波器的幅频响应对比分析  
A1 = BW*Fs/(2*pi);  
A2 = C1W*Fs/(2*pi);  
A3 = C2W*Fs/(2*pi);  
A4 = EW*Fs/(2*pi);  
A5 = YW*Fs/(2*pi);  
  
figure(7); % 画图  
subplot(1,1,1);plot(A1,abs(BH),A2,abs(C1H),A3,abs(C2H),A4,abs(EH),A5,abs(YH));grid;  
xlabel('频率／Hz');  
ylabel('频率响应幅度');  
legend('butter','cheby1','cheby2','ellip','yulewalk');  

=====================================






去直流
======
	对buffer取平均值，然后每一项都减去该平均值

sum = ∑cX
avg = sum/N
Cx -= avg

	为了保证buffer与buffer的avg不会“断开”，使用循环队列。

sum = ∑cX
cX = cX+1,cN = C1
sum = sum -c1 + cN
avg = sum/N
Cx -= avg

	引入上一次的avg,逐个消除计算平均值.使用前面N个数的avg来计算当前的直流量，N的取舍不一定，过大会比较迟钝，过小抖动比较厉害，如果是交流。特性类似滤波器，该过程会计算得到很多个平均量，去之流过程放在数字滤波器后面会比较好，减小脉冲给直流量带来干扰。

avg = InitValue
sum = avg * N
sum = sum + cX - avgX
avgX = sum / N
cX -= avgX

	注：对交流信号，N最好是最大周期整数倍。


信号的频率
=========
	一般测量的是最大信号的频率

FFT频域分析
----------

累计周期
--------
	对信号进行数字滤波，截至频率范围要尽可能小，但包含需要测量频率的衰减不宜过大。累计时间i,搜寻顶点iX-1<iX,iX>iX+1，频率为出现一个顶点与使用时间的比值。
累计多个顶点数来提高频率的精度，过高没有意义，对频率序列进行滤波没有意义
	对交流信号可以使用累计过零点计算。
	对之流进信号滤波并去之流后也可以使用过零点计算。

硬件方法
-------
	使用硬件帮助完成频率计量，避免软件反复比较。
	将信号通过大电阻和二极管链接到IO上，IO成输入状态，上升沿出发记录时间，两次触发间隔时间为一个周期。
	注：仅仅适用于测量的频率的信号幅度大于最小二极管管压降上下信号。注意减小该硬件对信号源的影响




有效值
=====

均方值
-----
	

-----
准同步计算
---------
	准同步系数

	计算方法



电量
====





