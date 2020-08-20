#include<bits/stdc++.h>
using namespace std;

string Divisor="100000100110000010001110011010111",FCS,reminder1,error_frame,Frame,temp1;
char data[1000],data_frame[100];
int data_size,frame_send=0,current_data=0,i,temp,error=0,ack=0;
double number_of_frame,data_per_frame=64;

void get_data();
void transmitter();
void transmission(string Frame);
void frame_genarte();
void receiver(string f,int e);
string CRC(string Dividend);

int main(){
    get_data();
    transmitter();
}

///taking data from file
void get_data(){
    ifstream input("input.txt");
    char bit='0';
    data_size=0;
    while(input>>bit){
        data[data_size]=bit;
        data_size++;
    }
    input.close();
    number_of_frame=ceil(data_size/data_per_frame); ///calculating number of frame
}

void transmitter(){
    for(int j=0;j<number_of_frame;j++){
        cout<<endl<<"------------------------------------------------------------------"<<endl;
        cout<<"Packet number: "<<frame_send<<" Sequence no: "<<ack<<endl;
        cout<<"------------------------------------------------------------------"<<endl<<endl;
        frame_genarte(); ///generating frame
        frame_send++;
    }
}

void frame_genarte(){
    //cout<<"=================================================================="<<endl;
   cout<<"SENDER:: Frame generating::"<<endl;
    if(frame_send<number_of_frame){
        for(i=0;i<data_per_frame;i++){
            data_frame[i]=data[current_data++];
        }
    }
    else{
        temp=data_size-current_data; ///for last frame
        for(i=0;i<temp;i++){
            data_frame[i]=data[current_data++];
        }
    }
    FCS ="";
    Frame="";
    while(FCS.size()<Divisor.size()-1){
        FCS.insert(0,"0");
    }
    string Frame = data_frame+FCS;///added initial FCS and data or data word
    FCS=CRC(Frame);
    Frame=data_frame+FCS; ///final FCS added here
    cout<<"After adding FCS: "<<Frame<<endl;
    transmission(Frame);

}

void transmission(string Frame){

    int r=rand()%2;     ///1  error...0 no error
    cout<<endl<<"SENDER:: Transmitting data ==========>\t\t";
    if(r==1){

        if(error<3){        ///maximum 3 times error will occur
                error++;
                int e=rand()%3;     ///0=lost data 1=lost ack 2=data error
                if(e==0){        ///lost data
                    receiver(Frame,e);
                }
                if(e==1){       ///lost ack
                    receiver(Frame,e);
                }
                if(e==2){       ///data error
                    int p=rand()%97;
                    error_frame=Frame;
                    if(error_frame[p]=='1'){
                        error_frame[p]='0';
                    }else{
                        error_frame[p]='1';
                    }    ///changing one bit
                     receiver(error_frame,e);
                }
        }
        else{
            receiver(Frame,3);
        }
    }
    else
        receiver(Frame,3);
}

 void receiver(string f,int e){

    switch(e){
    case 0:
        cout<<"RECEIVER:: Data lost!!****************No Acknowledgement"<<endl;
        cout<<endl<<"SENDER:: Resending Frame "<<endl;
        transmission(f);

        break;
    case 1:
        cout<<"\nSENDER:: Lost Acknowledgement "<<endl<<endl;
        cout<<"SENDER:: Resending Frame"<<endl;
        transmission(f);
        break;
    case 2:
        {
          ///checking error
          temp1=CRC(f);
          bool error1 = false;
          for(int i=0; i<temp1.size();i++){
            if(temp1[i]=='1')
                     error1 = true;
          }
          if(error1){
            cout<<"RECEIVER:: Error in data"<<endl<<endl;
          }
        cout<<"SENDER:: Resending Frame "<<endl;
        transmission(f);
        break;
    }

    case 3:
        ack=(ack==0)?1:0;
        cout<<"RECEIVER:: <========== ACK "<<ack<<endl<<endl;
        break;
    }
}


string CRC(string Dividend)
{
    string Temp="",Reminder;
    int FCSLen = Divisor.size()-1;

    for(int i=0; i<Dividend.size(); i++)
    {
        if(Temp.size()<Divisor.size())
            Temp.push_back(Dividend[i]);

        if(Temp.size()==Divisor.size())
        {
            Reminder="";
            for(int i=0; i<Temp.size(); i++)
                if(Temp[i] == Divisor[i])
                    Reminder.push_back('0');
                else
                    Reminder.push_back('1');
            while(Reminder[0]=='0')
                Reminder.erase(0,1);
            Temp = Reminder;
        }
    }
    while(Temp.size()<FCSLen)
        Temp.insert(0,"0");
    return Temp;
}


