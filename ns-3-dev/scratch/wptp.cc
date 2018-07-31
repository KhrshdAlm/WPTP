/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 * Date: 25.06.2018
 * Institute: DFKI GmbH
 * Author: Khurshid Alam <khurshid.alam@outlook.de>
 * Precision Time Protocol(PTP) for synchronization of WLAN (PCF)
 */

#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/command-line.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/ssid.h"
#include "ns3/wifi-mac-header.h"
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

using namespace ns3;
using namespace std;


#define SNAME "/mysem"

NS_LOG_COMPONENT_DEFINE ("WifiPtp");

// Msg Enumeration
enum MSG{
  MSMT,
  ACK
};


// State Enumeration
enum STATE{
  ACTIVE,
  WAITING,
  SYNCED
};
void recvPkt(NodeContainer wifiStaNode, Ipv4Address ipAp, uint16_t portAp);


//Global Variables

//static sem_t semPrepareExit;
Ptr<Socket> socketAp;
vector < Ptr<Socket> > socketSta;
uint32_t nWifiUsers = 3, cntr = 1;

NodeContainer wifiStaNodes, wifiApNode;


vector < vector <Time> > t1(nWifiUsers), t11(nWifiUsers), t2(nWifiUsers), t3(nWifiUsers), t4(nWifiUsers);

// Function sends ACK frame after receiving the measurement frame
void sendAck (uint32_t sid, uint32_t did)
{
  //t3 = 
  //usleep(100);

  stringstream msgy;
  msgy << did << '#' << sid << '#' << 0 << '#' << ACK << '#' << 0;
  Ptr<Packet> ack_pkt = Create<Packet>((uint8_t*) msgy.str().c_str(), 250);
  
  
  socketSta[did]->Send( ack_pkt );
  t3[did].push_back(wifiStaNodes.Get(did)->GetLocalTime());
  cout << "ACK sent! " << "t3: " << t3[did][t3[did].size()-1] << endl;


  for (Time ct = NanoSeconds( Simulator::Now() ); NanoSeconds( Simulator::Now() ) - ct < 25000;){

  //----------------------------------------------------------------------------
  if (socketAp->Listen() < 0){
    nanosleep((const struct timespec[]){{0, 500L}}, NULL);
    cout << "connection failed" << endl;
    continue;
  } 
  else {

  static unsigned int call_count = 0;
  Ptr<Packet> pktRecvd = socketAp->Recv();

  cout << "size: " << pktRecvd->GetSize() << endl;
  uint8_t *buffer = new uint8_t[pktRecvd->GetSize()]; 
  pktRecvd->CopyData (buffer, pktRecvd->GetSize());

  //cout<<buffer<<endl;
  uint8_t msgType = buffer[6];
  sid = buffer[0] - '0';
  did = buffer[2] - '0';
  
  if (msgType == '0'){
    
    //Simulator::Schedule( NanoSeconds( 100000000 ), &sendAck );
    t2[did].push_back(wifiStaNodes.Get(0)->GetLocalTime());
    Simulator::Schedule(Seconds (0.0000010), &sendAck, sid, did); // send ack after 1us

    cout << "src: " << sid << " dst: " << did << " : MSMT " << "t2: " << t2[did][t2[did].size()-1] << /*msgType << */endl;
  }
  else{
    t4[sid].push_back(wifiApNode.Get(0)->GetLocalTime());
    cout << "src: " << sid << " dst: " << did << " : ACK " << "t4: " << t4[sid][t4[sid].size()-1] << /*msgType << */endl;
    
  }
  call_count++;
  cout << call_count << endl << "________________________" << endl;
  break;

  //-----------------------------------------------------------------------------
}
} 

}//sendAck ends

// Function to send the measurement request frame
void sendMsmtReq (/*NodeContainer wifiApNode,*/ NodeContainer wifiStaNode, Ipv4Address ipAp, uint16_t portAp) // arguments are no more requirted since the nodes are declred global, station node required here only to know the specific sending node
{

  /*
  t1.push_back(wifiApNode.Get(0)->GetLocalTime());
  auto tid = wifiApNode.Get(0)->GetId();
  cout << t1[0] << ": " << NanoSeconds(t1[0]) << ":" << tid << endl;
  */
  // t2.push_back(wifiStaNode.Get(0)->GetLocalTime());
  uint32_t did = wifiStaNode.Get(0)->GetId(); // destination id
  uint32_t sid = wifiApNode.Get(0)->GetId(); // source id
  
  //cout << tid << endl;
  //Ptr<Packet> msmt_pkt = composeReqPkt(tid);
  // Sending the measurement request  
  
  stringstream msgx;
  msgx << sid << '#' << did << '#' << 0 << '#' << MSMT << '#' << 0;
  Ptr<Packet> msmt_pkt = Create<Packet>((uint8_t*) msgx.str().c_str(), 250);
  socketAp->Send( msmt_pkt );
  
  t1[did].push_back(wifiApNode.Get(0)->GetLocalTime());
  
  cout << "MSMT sent to: " << did << " t1: " << t1[did][t1[did].size()-1] << endl;
  //cout << msgx.str().c_str() << endl;
  //msgx << '\0' ;
  //usleep(1);  
  recvPkt(wifiStaNode, ipAp, portAp);

} // sendmsmtreq

void recvPkt(NodeContainer wifiStaNode, Ipv4Address ipAp, uint16_t portAp)
{

  uint32_t did = wifiStaNode.Get(0)->GetId(); // destination id
  uint32_t sid = wifiApNode.Get(0)->GetId(); // source id


  Time ct = NanoSeconds( Simulator::Now() );

  //for (; NanoSeconds( Simulator::Now() ) - ct < 34000;){
  //----------------------------------------------------------------------------
  if(socketSta[did]->Connect( InetSocketAddress(ipAp, portAp) ) < 0){
    cout << "connection failed" << endl;
    exit(0);
  }
  //else
  //  cout << "connected to station" << endl;

  //cout << NanoSeconds( Simulator::Now() ) << ct << endl;

  static unsigned int call_count = 0;

  //else{
  
  Ptr<Packet> pktRecvd = socketSta[did]->Recv();
  if (pktRecvd == 0){
    //cout << "no msg recvd" << endl;
    Simulator::Schedule(Seconds (0.000001), &recvPkt, wifiStaNode, ipAp, portAp/*, wifiApNode, wifiStaNodes, nWifiUsers, simulationTime*/);
  }

  else{



  cout << "size: " << pktRecvd->GetSize() << endl;
  uint8_t *buffer = new uint8_t[pktRecvd->GetSize()]; 
  pktRecvd->CopyData (buffer, pktRecvd->GetSize());

  cout << buffer << endl;

  uint8_t msgType = buffer[6];
  sid = buffer[0] - '0';
  did = buffer[2] - '0';
  
  if (msgType == '0'){
    
    //Simulator::Schedule( NanoSeconds( 100000000 ), &sendAck );
    t2[did].push_back(wifiStaNodes.Get(0)->GetLocalTime());
    Simulator::Schedule(Seconds (0.0000010), &sendAck, sid, did); // send ack after 1us

    cout << "src: " << sid << " dst: " << did << " : MSMT " << "t2: " << t2[did][t2[did].size()-1] << /*msgType << */endl;
  }
  else{
    t4[sid].push_back(wifiApNode.Get(0)->GetLocalTime());
    cout << "src: " << sid << " dst: " << did << " : ACK " << "t4: " << t4[sid][t4[sid].size()-1] << /*msgType << */endl;
    
  }
  call_count++;
  cout << call_count << endl << "________________________" << endl;

  //-----------------------------------------------------------------------------
  //break;
}

}

// Function called once a packet is received at any socket
/*
void recvMsg (Ptr<Socket> socket)
{
  //t2 = msmtRecvd
  //t4 = ackRecvd
  //sem_t *sem = sem_open(SNAME, 0);

  static unsigned int call_count = 0;
  Ptr<Packet> pktRecvd = socket->Recv();

  cout << "size: " << pktRecvd->GetSize() << endl;
  uint8_t *buffer = new uint8_t[pktRecvd->GetSize()]; 
  pktRecvd->CopyData (buffer, pktRecvd->GetSize());

  //cout<<buffer<<endl;
  uint8_t msgType = buffer[6];
  uint32_t sid = buffer[0] - '0';
  uint32_t did = buffer[2] - '0';
  
  if (msgType == '0'){
    
    //Simulator::Schedule( NanoSeconds( 100000000 ), &sendAck );
    t2[did].push_back(wifiStaNodes.Get(0)->GetLocalTime());
    Simulator::Schedule(Seconds (0.0000010), &sendAck, sid, did); // send ack after 1us

    cout << "src: " << sid << " dst: " << did << " : MSMT " << "t2: " << t2[did][t2[did].size()-1] << msgType << endl;
  }
  else{
    t4[sid].push_back(wifiApNode.Get(0)->GetLocalTime());
    cout << "src: " << sid << " dst: " << did << " : ACK " << "t4: " << t4[sid][t4[sid].size()-1] << msgType << endl;
    
  }
  call_count++;
  cout << call_count << endl << "________________________" << endl;

  //sem_post(sem);


}// end of recvMsg()

*/

// Sceduled in main(), the initilaization of the protocol
void startProtocol(/*NodeContainer wifiApNode, NodeContainer wifiStaNodes, uint32_t nWifiUsers, double simulationTime*/Ipv4Address ipAp, uint16_t portAp)
{
  /*
  cout <<nWifiUsers<<" : "<<simulationTime<<endl;
  auto ipv4Ap = wifiApNode.Get(0)->GetObject<Ipv4> ();
  cout << ipv4Ap << endl;
  const auto address = ipv4Ap->GetAddress (1, 0).GetLocal ();
  cout<<"#:"<<address<<endl;
  for (uint32_t index = 0; index < nWifiUsers; ++index)
  {
    auto ipv4Sta = wifiStaNodes.Get(index)->GetObject<Ipv4> ();
    cout << ipv4Sta << endl;
    const auto address = ipv4Sta->GetAddress (1, 0).GetLocal ();
    cout<<index<<":"<<address<<endl;
  }
  */

  //srcId = nWifiUsers;
  //while(cntr < 4)
  //{

    //sem_t *sem = sem_open(SNAME, O_CREAT | O_EXCL, 0644, 3);
    //sem_unlink (SNAME);
    
    //sem_init(&sem, 1, 1);

    uint32_t i = 0, j = 0;
    for (; i < nWifiUsers; ++i, ++j)
    {        
      
      Simulator::Schedule(Seconds (.0), &sendMsmtReq, wifiStaNodes.Get(i), ipAp, portAp/*, wifiApNode, wifiStaNodes, nWifiUsers, simulationTime*/);      
      //sendMsmtReq(/*wifiApNode,*/ wifiStaNodes.Get(i), ipAp, portAp);
      //socketSta[i]->Accept();
      //Ptr<Packet> pktRecvd = socketSta[i]->Recv();        
      //usleep(100);
      //if (i >= nWifiUsers)
      //{
      //  i = 0;
      //}      
      //sem_wait(sem);


    }

   /* 
    for (i=0; i < nWifiUsers; ++i, ++j)
    {
      sem_init(&semPrepareExit, 1, 0);
      Simulator::Schedule(Seconds (j * 0.00005), &sendMsmtReq, wifiStaNodes.Get(i)); //, wifiApNode, wifiStaNodes, nWifiUsers, simulationTime);
      sem_post(&semPrepareExit);
    }
  */

    //cout << i ;
    //j = i*0.00005;    
    //cntr++;
  //}  


}//end of startProtocol

int main (int argc, char *argv[])
{
  
  uint32_t packetSize = 250; // bytes
  uint64_t cfpMaxDuration = 65536; //microseconds
  double simulationTime = 10; //seconds
  
  CommandLine cmd;
  cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
  cmd.AddValue ("nWifiUsers", "Number of receivers", nWifiUsers);
  cmd.AddValue ("cfpMaxDuration", "CFP max duration in microseconds", cfpMaxDuration);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.Parse (argc, argv);

  //NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifiUsers);

  //NodeContainer wifiApNode;
  wifiApNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  WifiMacHelper mac;

  Ssid ssid = Ssid ("wifi-ptp");
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate54Mbps"), "ControlMode", StringValue ("OfdmRate24Mbps"));

  NetDeviceContainer staDevices;
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  NetDeviceContainer apDevice;
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true));               
               //"CfpMaxDuration", TimeValue (MicroSeconds (cfpMaxDuration)));
  apDevice = wifi.Install (phy, mac, wifiApNode);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint32_t i = 0; i < nWifiUsers; i++)
    {
      positionAlloc->Add (Vector (1.0, 0.0, 0.0));
    }
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNodes);

  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer StaInterface;
  StaInterface = address.Assign (staDevices);

  Ipv4InterfaceContainer ApInterface;
  ApInterface = address.Assign (apDevice);


  // ports for socket
  vector < uint32_t > portSta (nWifiUsers);
  //vector < uint32_t > portAp (nWifiUsers);
  
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");

  auto ipv4Ap = wifiApNode.Get(0)->GetObject<Ipv4> ();
  Ipv4Address ipAp = ipv4Ap->GetAddress (1, 0).GetLocal ();
  uint16_t portAp = (nWifiUsers + 1) * 100;
  socketAp = Socket::CreateSocket(wifiApNode.Get(0), tid);
  socketAp->Bind( InetSocketAddress(ipAp, portAp));
  
  //cout << ipAp;
  
  for (uint32_t i = 0; i < nWifiUsers; ++i)
  {
    
    auto ipv4Sta = wifiStaNodes.Get(i)->GetObject<Ipv4> ();
    auto ipSta = ipv4Sta->GetAddress (1, 0).GetLocal ();
    portSta[i] = i * 100 + 100;

    
    

    cout << ipAp << "\t" << portAp << endl;
    cout << ipSta << "\t" << portSta[i] << endl;


    socketSta.push_back( Socket::CreateSocket(wifiStaNodes.Get(i), tid) );
    socketSta[i]->Bind( InetSocketAddress(ipSta, portSta[i]) );
    socketSta[i]->Connect( InetSocketAddress(ipAp, portAp) );
    //socketSta[i]->SetRecvCallback( MakeCallback (&recvMsg) );

    socketAp->Connect( InetSocketAddress(ipSta, portSta[i]) );
    //socketAp[i]->SetRecvCallback( MakeCallback(&recvMsg) );


  }

  cout<<"Simulation Starts!"<<endl;
  Simulator::Schedule(Seconds (1.0), &startProtocol/*, wifiApNode, wifiStaNodes, nWifiUsers, simulationTime*/,ipAp,portAp);
  Simulator::Stop (Seconds (simulationTime + 1));
  Simulator::Run ();
  
  for (size_t i = 0; i < nWifiUsers; ++i)
  {
    //cout << endl << "User: " << i+1 << endl;
    for (size_t j = 0; j < t1[i].size(); ++j)
    {
      cout << "t1: " << t1[i][j] << "\t";
      cout << "t2: " << t2[i][j] << "\t";
      cout << "t3: " << t3[i][j] << "\t";
      cout << "t4: " << t4[i][j] << endl;
    }
  }

  phy.EnablePcap ("wptp11v", apDevice);
  phy.EnablePcap ("wptp11v", staDevices);

  cout << "_________________________________\n\tSimulation Ended!\n_________________________________" << endl;
  Simulator::Destroy ();
  return 0;
}