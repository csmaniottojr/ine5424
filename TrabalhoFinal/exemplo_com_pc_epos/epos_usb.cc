#include <utility/ostream.h>
#include <usb.h>

using namespace EPOS;

int main()
{
    OStream cout;
    USB usb;

    for(bool b=false;;b=(b+1)%2) {
    	for(int i = 0; i < 2; i++) {

    		cout << i << " - algum comando..." << endl;
    		bool fimAck = false;
    		while(!fimAck){
    			char c = usb.get();
    			cout << c; 
    			if(c == '\n'){
    				fimAck = true;
    			}
    		}

    	}
    	for(volatile int t=0;t<0xfffff;t++);
    }

    return 0;    
}