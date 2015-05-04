#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

#define CHECK_INTERVAL           10     //second

std::vector<std::string> ChannelList;   //epics channel name
std::vector<double>      HighLEVEL;     //
std::vector<double>      LowLEVEL;      //

time_t now;
time_t last;

void CheckEpicsData();
void PrintTime();
void AlarmSound();

int main(int argc, char* argv[])
{
  if(argc !=2){
    printf("Usage: %s <channel_list_file> \n",argv[0]);
    return 0;
  }
  
  // channel_list_file check
  std::ifstream ifs(argv[1]);
  if(!ifs){
    printf("cannot access %s: No such file\n",argv[1]);
    return 0;
  }
  
  // read channel list
  std::string line;
  while(ifs && getline(ifs,line)){
    if(line[0]=='#' || line.empty() ) continue;

    char c[256];
    double p1,p2;

    if( sscanf(line.c_str(),"%s %lf %lf",c, &p1, &p2) == 3 ){
      std::string name(c);
      // epics channel name
      ChannelList.push_back(name);
      LowLEVEL.push_back(p1);
      HighLEVEL.push_back(p2);
      //printf("%s, %f, %f\n",c,p1,p2);
    }   
  }
  ifs.close();

  // print channel list
  printf("================ EPICS channels ==================\n");
  for(int i=0; i<(int)ChannelList.size(); ++i){
    printf("%d %s   L=%f   H=%f\n", i+1, ChannelList[i].c_str(), LowLEVEL[i], HighLEVEL[i] );
  }
  printf("==================================================\n");

  last = time(0);
  
  while(1){
    now = time(0);
    
    CheckEpicsData();
    PrintTime();
    
    int rest = CHECK_INTERVAL - (time(0) - now);
    if( rest>0 ) sleep(rest);
  }
  
  return 0;
}


void CheckEpicsData()
{
  std::string caget = "caget -w 3 -t ";
  std::string cmdline;
  FILE* fp;
  char input[128];

  for(int i=0; i<(int)ChannelList.size(); ++i){
    cmdline = caget+ChannelList[i];
    fp=popen(cmdline.c_str(),"r");
    fgets(input, 128, fp);
    if(pclose(fp) !=0) continue;
    
    double data;
    if( sscanf(input,"%lf",&data) == 1){
      if(data < LowLEVEL[i]){
	printf("%s (val=%f) exceeds the LOW limit !!!!\n",ChannelList[i].c_str(), data);
	AlarmSound();
      }
      if(data > HighLEVEL[i]){
	printf("%s (val=%f) exceeds the HIGH limit !!!!\n",ChannelList[i].c_str(), data);
	AlarmSound();
      }
    }
    usleep(10000);
  }
}

void PrintTime()
{
  char s[256];
  struct tm *p = localtime(&now);
  strftime(s, 256, "%Y/%m/%d %H:%M:%S", p);
  printf("Last Check Time: %s\n",s);
}

void AlarmSound()
{
  system("sh script/call_alarm.sh");
}
