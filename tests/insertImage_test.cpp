#include <iostream>
#include "../include/Image.hpp"

using namespace std;


int main()
{
	Image slave(30,30);
	cout << "slave: " << slave.width() << "x" << slave.height() << endl;
	slave.penColor(255,0,0);
	slave.rect(0,0,slave.width()-1,slave.height()-1);
	
	Image master(100,100);
	cout << "master: " << master.width() << "x" << master.height() << endl;
	master.penColor(50,50,255);
	master.rect(0,0,master.width()-1,master.height()-1);
	
	master.insertImage(20, 20, slave);
	master.insertImage(70, 70, slave);
	master.insertImage(20,-10, slave);
	
	master.resize(150,100);
	cout << "new master: " << master.width() << "x" << master.height() << endl;
	master.save_bmp("insertImage_test.bmp");
	
	return 0;
}
