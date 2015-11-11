#include "pathway.h"
#include "region.h"
#include "nfe_l.h"

class recognizer
{
	public:
	  recognizer(pathway * _pw);
	  int neuronsRegion(unsigned int i);

	private:
	  pathway *_pw;


};
