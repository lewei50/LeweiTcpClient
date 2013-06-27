LeweiTcpClient
==============
This is the the lite version of LeweiTcpClient,this version were tested on arduino UNO.

Using this library is the one of the most easiest way to

keep connected to www.lewei50.com to store your data and make your arduino "controllable" from the Internet/your Smart Phone.

Now,only 3 steps between you and your arduino controllable:

step1:define your apikey and gateway number in your .ino file.

step2:copy this line into your loop() function,client->keepOnline();

step3:bind your function you write in arduino to the name you defined on website.

(check more detail in the example file.)

then it's all under your controll.

because of the difference between arduino boards,we HIGHLY RECOMMENDED your using the lite version for your UNO board.
UNO is running on less memory(2K).

If you are using arduino mega or something else you can try the normal version here:

https://github.com/lewei50/LeweiTcpClient/

the lite version can only accept 0/1 parameter(s) from the website you defined on it.
the normal version can accecpt 0/1/2/3/4/5 parameter(s).
Enjoy it~

Email:gyangbo@gmail.com
