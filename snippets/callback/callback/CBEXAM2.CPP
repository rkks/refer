#include <iostream.h>
#include "callback.hpp"

class Button{
public:
	Button(const CBFunctor0 &uponClickDoThis):notify(uponClickDoThis)
		{}
	void click()
		{
		if(notify)
			notify();
		}
private:
	CBFunctor0 notify;
};

class CDPlayer{
public:
	void play()
		{cout << "Playing"<<endl;}
	void stop()
		{cout << "Stopped"<<endl;}
};

void main()
	{
	CDPlayer myCD;
	Button playButton(makeFunctor((CBFunctor0*)0,myCD,&CDPlayer::play));
	Button stopButton(makeFunctor((CBFunctor0*)0,myCD,&CDPlayer::stop));
	playButton.click();	//calls myCD.play()
	stopButton.click();  //calls myCD.stop()
	}


