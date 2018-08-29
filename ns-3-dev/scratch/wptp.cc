/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 * Date: 25.06.2018
 * Institute: DFKI GmbH
 * Author: Khurshid Alam <khurshid.alam@outlook.de>
 * Precision Time Protocol(PTP) for synchronization of WLAN (PCF - based)
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
#include "ns3/ipv4-routing-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include "ns3/double.h"

using namespace ns3;
using namespace std;


NS_LOG_COMPONENT_DEFINE ("WifiPtp");


// Msg Enumeration
enum MSG{
  MSMT,
  ACK
};


/*
// State Enumeration
enum STATE{
  ACTIVE,
  WAITING,
  SYNCED
};
*/


//Global Variables
bool mBusy = false;
bool aPending = false;
//Ptr<Socket> socketAp;
vector < Ptr<Socket> > socketSta, socketAp;
uint32_t nWifiUsers = 5, cntr = 3;

NodeContainer wifiStaNodes, wifiApNode;
vector < vector <Time> > t1(nWifiUsers), t11(nWifiUsers), t2(nWifiUsers), t3(nWifiUsers), t4(nWifiUsers);

// Function sends ACK frame after receiving the measurement frame
void sendAck (uint32_t sid, uint32_t did)
{
  //t3 = Ack sending time
  //usleep(100);

  stringstream msgy;
  msgy << did << '#' << sid << '#' << 0 << '#' << ACK << '#' << 0;
  Ptr<Packet> ack_pkt = Create<Packet>((uint8_t*) msgy.str().c_str(), 250);
  
  
  socketSta[did]->Send( ack_pkt );
  t3[did].push_back(wifiStaNodes.Get(did)->GetLocalTime());
  
  cout << NanoSeconds( Simulator::Now() ) << endl;

  cout << "ACK sent to AP at time (t3): " << t3[did][t3[did].size()-1] << endl;
  

}//sendAck ends

// Function to send the measurement request frame
void sendMsmtReq (NodeContainer wifiStaNode, Ipv4Address ipAp, uint16_t portAp) // arguments are no more requirted since the nodes are declred global, station node required here only to know the specific sending node
{

    /*
    t1.push_back(wifiApNode.Get(0)->GetLocalTime());
    auto tid = wifiApNode.Get(0)->GetId();
    cout << t1[0] << ": " << NanoSeconds(t1[0]) << ":" << tid << endl;
    */
  
  if (mBusy == false && aPending == false)
  {

    uint32_t did = wifiStaNode.Get(0)->GetId(); // destination id
    uint32_t sid = wifiApNode.Get(0)->GetId(); // source id

    stringstream msgx;
    msgx << sid << '#' << did << '#' << 0 << '#' << MSMT << '#' << 0;
    Ptr<Packet> msmt_pkt = Create<Packet>((uint8_t*) msgx.str().c_str(), 250);
    
    socketAp[did]->Send( msmt_pkt ); // send MSMT request
    
    t1[did].push_back(wifiApNode.Get(0)->GetLocalTime());
    
    cout << NanoSeconds( Simulator::Now() ) << endl;

    cout << "MSMT sent to Sta " << did << " at time (t1): " << t1[did][t1[did].size()-1] << endl;
    mBusy = true;  

  } 

  else 
  {
    //cout << NanoSeconds( Simulator::Now() ) << endl;
    Simulator::Schedule(NanoSeconds (500), &sendMsmtReq, wifiStaNode, ipAp, portAp); 

  }

} // sendMsmtReq


// Function called once a packet is received at any socket
void recvMsg (Ptr<Socket> socket)
{
  //t2 = msmtRecvd
  //t4 = ackRecvd

  //static unsigned int call_count = 0;

  Ptr<Packet> pktRecvd = socket->Recv();

  //cout << "size: " << pktRecvd->GetSize() << endl;
  uint8_t *buffer = new uint8_t[pktRecvd->GetSize()]; 
  pktRecvd->CopyData (buffer, pktRecvd->GetSize());

  //cout<<buffer<<endl;
  uint8_t msgType = buffer[6];
  uint32_t sid = buffer[0] - '0';
  uint32_t did = buffer[2] - '0';
  
  if (msgType == '0')
  {
    
    //Simulator::Schedule( NanoSeconds( 100000000 ), &sendAck );
    t2[did].push_back(wifiStaNodes.Get(0)->GetLocalTime());

    Simulator::Schedule(NanoSeconds (500), &sendAck, sid, did); // send ack after 1us  
    
    cout << "Received MSMT request" << endl; 

    cout << NanoSeconds( Simulator::Now() ) << endl;

    cout << "src: " << sid << " dst: " << did << " : MSMT " << "t2: " << t2[did][t2[did].size()-1] << endl;
    aPending = true; // signals the acknowledment is pending!

  }

  else
  {

    aPending = false; // signals the ack sent

    t4[sid].push_back(wifiApNode.Get(0)->GetLocalTime());
    cout << "Received ACK request" << endl;

    cout << NanoSeconds( Simulator::Now() ) << endl;

    cout << "src: " << sid << " dst: " << did << " : ACK " << "t4: " << t4[sid][t4[sid].size()-1] << endl;
    cout << "________________________" << endl;    
    
  }

/*  
  call_count++;
  cout << call_count << endl << "________________________" << endl;
*/

  mBusy = false;

}// end of recvMsg()


// Sceduled in main(), the initilaization of the protocol
void startProtocol(Ipv4Address ipAp, uint16_t portAp)
{
  while (cntr > 0) {

    uint32_t i = 0, j = 0;
    for (; i < nWifiUsers; ++i, ++j)
    {        
      
      Simulator::Schedule(Seconds (0), &sendMsmtReq, wifiStaNodes.Get(i), ipAp, portAp);      
  
    }

    cntr--;
  }

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
  channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.Set ("RxGain", DoubleValue( 0.0));
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
  WifiMacHelper mac;

  Ssid ssid = Ssid ("wifi-ptp");
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate54Mbps"), "ControlMode", StringValue ("OfdmRate54Mbps"));

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
  uint32_t port = 10;
  //vector < uint32_t > portAp (nWifiUsers);
  
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");

  auto ipv4Ap = wifiApNode.Get(0)->GetObject<Ipv4> ();
  Ipv4Address ipAp = ipv4Ap->GetAddress (1, 0).GetLocal ();
  
  //socketAp = Socket::CreateSocket(wifiApNode.Get(0), tid);
  //cout << ipAp;
  
  for (uint32_t i = 0; i < nWifiUsers; ++i)
  {
    
    auto ipv4Sta = wifiStaNodes.Get(i)->GetObject<Ipv4> ();
    auto ipSta = ipv4Sta->GetAddress (1, 0).GetLocal ();
    
  /*  
    cout << ipAp << "\t" << portAp << endl;
    cout << ipSta << "\t" << portSta[i] << endl;
  */

    socketSta.push_back( Socket::CreateSocket(wifiStaNodes.Get(i), tid) );
    socketSta[i]->Bind( InetSocketAddress(ipSta, port) );

    socketAp.push_back( Socket::CreateSocket(wifiApNode.Get(0), tid));
    socketAp[i]->Bind( InetSocketAddress(ipAp, port+i));

    socketSta[i]->Connect( InetSocketAddress(ipAp, port+i) );
    socketSta[i]->SetRecvCallback( MakeCallback (&recvMsg) );
    
    socketAp[i]->Connect( InetSocketAddress(ipSta, port) );
    socketAp[i]->SetRecvCallback( MakeCallback(&recvMsg) );

  }
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  cout << "_________________________________\n\tSimulation Starts!\n_________________________________" << endl;
  

  //PcapHelper pcapHelper;
  //Ptr<PcapFileWrapper> file = pcapHelper.CreateFile ("wptp.pcap", std::ios::out, PcapHelper::DLT_PPP);
  //staDevices.Get (1)->TraceConnectWithoutContext ("PhyWptp", MakeBoundCallback (&recvMsg, file));

  phy.EnablePcap ("wptpAp11v", apDevice);
  phy.EnablePcap ("wptpSta11v", staDevices);

  Simulator::Schedule(Seconds (1.0), &startProtocol, ipAp, port);
  Simulator::Stop (Seconds (simulationTime + 1));
  
  Simulator::Run ();
  
  for (size_t i = 0; i < nWifiUsers; ++i)
  {
    
    for (size_t j = 0; j < t1[i].size(); ++j)
    {
  
      cout << "t1: " << t1[i][j] << "\t";
      cout << "t2: " << t2[i][j] << "\t";
      cout << "t3: " << t3[i][j] << "\t";
      cout << "t4: " << t4[i][j] << endl;
  
    }
  
  }

  
  //phy.EnablePcap ("wptpSta11v", staDevices);

  cout << "_________________________________\n\tSimulation Ended!\n_________________________________" << endl;
  Simulator::Destroy ();
  return 0;


}