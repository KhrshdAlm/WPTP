/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"  
#include "ns3/drop-tail-queue.h"
#include <string>
#include <sstream>
#include "ns3/simple-wireless-tdma-module.h"
#include "ns3/dsdv-module.h"

// Default Network Topology
//
//   Wifi 10.1.1.0
//
//            AP
//  *    *    *    *
//  |    |    |    |    
//  |    |    |    |
//  n5   n6   n7   n0
//
////////////////////////////

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

int 
main (int argc, char *argv[])
{
  bool enableFlowMonitor=true; 
  uint32_t nwifi = 10;
  //uint32_t nwifi1 = 2;
  //uint32_t nwifi2 = 1;
  uint32_t nserver = 1;
  uint32_t cap[11]={125,500,201,135,137,201,111,96,109,122,200};
  //double cap[15]={0.001};
  uint32_t load[11]={0,4,4,4,2,2,4,4,4,2,4};
  
  SeedManager::SetRun (2); 

  CommandLine cmd;
  cmd.AddValue ("nwifi", "Number of wifi nodes and one hops", nwifi);
  //cmd.AddValue ("nwifi1", "Number of wifi STA devices at two hops case 1", nwifi1);
  //cmd.AddValue ("nwifi2", "Number of wifi STA devices at two hops case 2", nwifi2);

  cmd.Parse (argc,argv);
  /**************************Creating different node container***********************/
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nwifi);
  //NodeContainer wifiStaNodes1;
  //wifiStaNodes1.Create (nwifi1);
  //NodeContainer wifiStaNodes2;
  //wifiStaNodes2.Create (nwifi2);
  NodeContainer sinkNode;
  sinkNode.Create (nserver); // sink

  /**************************Wifi STA Device assignment***********************/

  NodeContainer allNodes = wifiStaNodes; 
  allNodes.Add (sinkNode);
  //allNodes.Add (wifiStaNodes1);
  //allNodes.Add (wifiStaNodes2);
  TdmaHelper tdma = TdmaHelper (allNodes.GetN (),allNodes.GetN ()); 
  //In this case selected, numSlots = nodes
  TdmaControllerHelper controller;
  controller.Set ("SlotTime", TimeValue (MicroSeconds (1000)));
  controller.Set ("GaurdTime", TimeValue (MicroSeconds (0)));
  controller.Set ("InterFrameTime", TimeValue (MicroSeconds (0)));
  NetDeviceContainer staDevices;
  for(uint16_t i=0 ; i < allNodes.GetN() ; i++)
  {
    std::ostringstream oss;
    oss << cap[i] << "Mbps";
    controller.Set ("DataRate", DataRateValue (oss.str()));
    tdma.SetTdmaControllerHelper (controller);
    NetDeviceContainer dev = tdma.Install (allNodes.Get(i));
    staDevices.Add(dev);
  }

  /******************************WIFI STA Mobility*****************************/
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.InstallAll ();

  /***************************************END**********************************/
  /*******************************IP assignment *******************************/
  InternetStackHelper stack;
  stack.InstallAll ();

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer staInterfaces = address.Assign (staDevices);
  /***************************************END**********************************/

  /***************** Routing table for WIFI --> AP for WIFI ****************/
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  for (uint32_t u = 0; u < wifiStaNodes.GetN (); ++u)
  {
    Ptr<Node> wifistanode = wifiStaNodes.Get (u);
    Ptr<Ipv4StaticRouting> apStaticRouting = ipv4RoutingHelper.GetStaticRouting (wifistanode->GetObject<Ipv4> ());
    apStaticRouting->AddNetworkRouteTo (Ipv4Address ("10.1.1.1"), Ipv4Mask ("255.255.255.0"), 1);
  }
/*
  for (uint32_t u = 0; u < wifiStaNodes1.GetN (); ++u)
  {
    Ptr<Node> wifistaNode = wifiStaNodes1.Get (u);
    Ptr<Ipv4StaticRouting> apStaticRouting = ipv4RoutingHelper.GetStaticRouting (wifistaNode->GetObject<Ipv4> ());
    apStaticRouting->AddHostRouteTo(Ipv4Address("10.1.1.1"),Ipv4Address("10.1.1.2"), 1);
  }

  for (uint32_t u = 0; u < wifiStaNodes2.GetN (); ++u)
  {
    Ptr<Node> wifistaNode = wifiStaNodes2.Get (u);
    Ptr<Ipv4StaticRouting> apStaticRouting = ipv4RoutingHelper.GetStaticRouting (wifistaNode->GetObject<Ipv4> ());
    apStaticRouting->AddHostRouteTo(Ipv4Address("10.1.1.1"),Ipv4Address("10.1.1.3"), 1);	
  }
*/

  /******************************************************************************/
  /*********************************** DropTailQueue*****************************/
  /*DropTailQueueHelper droptail;
  droptail.set("MaxPackets",UintegerValue(100));
  droptail.InstallAll ();*/
  /**********************************Onoff-application***************************/
  

  uint16_t dlPort = 3333;
  ApplicationContainer clientApps, serverApps;
  Ptr<UniformRandomVariable> startTimeSeconds = CreateObject<UniformRandomVariable> ();
  startTimeSeconds->SetAttribute ("Min", DoubleValue (0.0));
  startTimeSeconds->SetAttribute ("Max", DoubleValue (0.01));

  PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
  serverApps.Add (dlPacketSinkHelper.Install (sinkNode.Get(0)));
  Ipv4Address wifiApNodesaddr = staInterfaces.GetAddress(0);

  for(uint16_t i=1 ; i < allNodes.GetN() ; i++)
  {
    OnOffHelper dlClient ("ns3::UdpSocketFactory", Address (InetSocketAddress (wifiApNodesaddr, dlPort)));
    std::ostringstream oss;
    oss << load[i] << "Mbps";
    std::string var = oss.str();
    dlClient.SetConstantRate (DataRate (var), 1024);
    clientApps.Add (dlClient.Install (allNodes.Get(i)));
    Time startTime = Seconds (startTimeSeconds->GetValue ()); // Will start at different time
    serverApps.Start (startTime);
    clientApps.Start (startTime);
  }

  /**************************************End****************************************************/

  Simulator::Stop (Seconds (10.0));

/*************************************Flow Monitor *******************************************/
  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();

  Simulator::Run ();
   if (enableFlowMonitor)
  {
    std::cout <<"Tx\tRx\tTxPkt\tRxPkt\tThput\tPkt Loss(%)\tdelay\tJitter\tHops\n------------------------------------------------\n";
    flowMonitor->CheckForLostPackets (); 
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {     
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
          double deltaTime = i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds();
          std::cout << t.sourceAddress << "\t";
          std::cout << t.destinationAddress << "\t";
          std::cout << i->second.txPackets << "\t";
          std::cout << i->second.rxPackets << "\t";
          std::cout << i->second.rxBytes * 8.0 / deltaTime / 1024 << "\t";
          std::cout << (double)(i->second.txPackets - i->second.rxPackets)/i->second.txPackets*100 << "\n";  
        /*std::cout << i->second.delaySum.GetMilliSeconds()/i->second.rxPackets<< "\t"; 
          std::cout << i->second.jitterSum.GetMilliSeconds()/(i->second.rxPackets-1)<< "\t"; 
          std::cout << 1+(i->second.timesForwarded /i->second.rxPackets)<< "\n";   
        */
    }
  }
/****************************************END**************************************************/
  Simulator::Destroy ();
  return 0;
}
