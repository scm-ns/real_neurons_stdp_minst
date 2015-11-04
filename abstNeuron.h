#include <iostream>
#include <vector>



enum FIRINGBIT
{
    FLAT = 0 ,SPIKE = 1
};


class abstNeuron
{


	virtual	bool tick() = 0 ;
	virtual bool readyForTick() = 0;
	virtual int getNOutputToSend() = 0 ;
	virtual FIRINGBIT getOutput() = 0 ;
    	virtual FIRINGBIT getInput() = 0 ;
	virtual void stimulus() = 0 ;
	virtual void setId(int id) = 0 ;
	virtual int getId() = 0 ;
	virtual bool setDebug(bool f ) = 0 ;
	virtual bool isTickComplete() = 0 ;
	virtual int8_t activity() = 0 ;
	virtual int8_t activityMax() = 0 ;
	virtual void connect(abstNeuron * n1 , double weight)  = 0 ;
	virtual void setOutputneuron(abstNeuron* _neuron) = 0 ;





};




