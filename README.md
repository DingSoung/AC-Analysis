###说明
该项目包含了对离散的数字信号的常用处理算法，包括傅立叶变换，频率计量，有效值计量。可用于示波器、电表、采集模块，信号分析仪等。另外简要说明了电压，电流，有功功率，无功功率，视在功率，四象限功率，功率因，功率因数，功率谱，和电量累计的算法。基于C语言。侧重计算方法，减小误差，提高稳定性。

####去直流
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

####有效值
一般的计算一段序列有效值，使用均方

	sum = ∑(Xi*Xi)
	rms = √(sum/N)

需要得到稳定的rms,Xi的长度需要为Xi周期的整数倍，如果Xi的频率不确定，就需要先计算得到的的频率，假设预先计算到了整数频率f

	sum = (Xi*Xi) + .. + (X(i+f)*X(i+f))
	rms = √(sum/f)

实际计算精确的frequency需要的长度要比rms长很多倍，frequency的结果更新要比rms慢，当前要计算的rms使用的f是用包含以前的序列的，而且frequency不一定是整数，所以计算rms不能过度依赖f.就需要使用准同步计算。
复化求积准同步系数仿真，当计算有效值时，ci为采样平方之后的系数,采样点值为Xi,有效值为A,周期采样点数为m，采样周期数为n.则电流有效值为

	rms=√(∑(Xi * Xi * Coei)/pow(m,n))

如fs为采样频率、连续等间隔采样三个周期,准同步采样算法可知应该总共采样点数和信号为

	n * (fs/f - 1) + 1
	x = √2 A * cos(2 * π/T * t)

Matlab实验,修改频率f对比结果

	clear all;clc  
	fs = 4000		%采样频率
	f  = 50;		%信号频率
	A  = 220;		%幅度有效值
	ph = 0;			%相位
	dc = 0;			%直流量
	n  = 4;			%周期数
	m  = round(fs/f);	%每个周期的点数
	c  = [ones(1,m)];	%生成准同步系数
	c1 = c;
	for i = 1 : n-1
	    c = conv(c,c1)
	end
	t  = 1 : n * (fs/f - 1) + 1;			%时间轴
	x = A * sqrt(2) * cos((2 * pi)/(fs / f) * t);	%模拟波形
	rms = sqrt(sum(x .* x .* c ) / (m ^ n))		%计算结果
	rms2= sqrt(sum(x .* x)/(m * n))			%对比结果

####数字滤波器
I2R数字滤波器
包括输入和输出流，参考标准算法，使用Matlab FDAtool生成滤波器参数
没有浮点单元的，使用增益进行整形运算

	<...>


####信号的频率
一般测量的是最大信号的频率

	<...>

累计周期
对信号进行数字滤波，截至频率范围要尽可能小，但包含需要测量频率的衰减不宜过大。累计时间i,搜寻顶点iX-1<iX,iX>iX+1，频率为出现一个顶点与使用时间的比值。
累计多个顶点数来提高频率的精度，过高没有意义，对频率序列进行滤波没有意义
对交流信号可以使用累计过零点计算。
对之流进信号滤波并去之流后也可以使用过零点计算。

	<...>

硬件方法
使用硬件帮助完成频率计量，避免软件反复比较。
将信号通过大电阻和二极管链接到IO上，IO成输入状态，上升沿出发记录时间，两次触发间隔时间为一个周期。
注：仅仅适用于测量的频率的信号幅度大于最小二极管管压降上下信号。注意减小该硬件对信号源的影响

	<...>


####傅里叶展开
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
	x =  A1*sqrt(2)*cos(2*pi*f1*t) + A2*sqrt(2)*cos(2*pi*f2*t);	%产生信号
	x = x + 300*randn(size(t));			% 混入噪声信号
	subplot(311)
	plot(t(1:1000),x(1:1000))			%显示前1000个点
	xlabel('time (S)')
	subplot(312)
	plot(t(1:200)*1000,x(1:200))			%前200个点
	xlabel('time (mS)')
	N = 2^nextpow2(n);				% 求得最接近总采样点的2^n,也可以是指定的长度
	X = fft(x,N)/n;					% fft变换(除以总点数是为了精确看出原始信号幅值
	F = fs/2*(0,N/2);%linspace(0,fs/2,N/2+1);	% 频率轴(只画到Fs/2即可,y为实数,一半是对称的)
	subplot(313)					% 画出频率幅度图形
	plot(F,2*abs(X(1:N/2+1)))
	xlabel('Frequency (Hz)')
	ylabel('|Y(f)|')

FFTW库大致是先用fftw_malloc分配输入输出内存，然后输入数据赋值，然后创建变换方案（fftw_plan），然后执行变换（fftw_execute），最后释放资源，还是比较简单的
使用FFTW库的函数 fftw_plan_dft_r2c_1d()，另逆变换为fftw_plan_dft_c2r_1d()
还可以针对特殊条件进行优化：固定分析的长度，如1024，将sin cos改成查表法等。再就是利用硬件优势了，如使用汇编，操作寄存器使用硬件DSP等

	<...>

FFT频域分析

	<...>


####累计
<...>
