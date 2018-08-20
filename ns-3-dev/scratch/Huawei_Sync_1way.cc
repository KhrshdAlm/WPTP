/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017
 *
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
 *
 * Author: Sebastien Deronne <sebastien.deronne@gmail.com>
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
#include "timestamptag.h"


using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("Huawei_Sync_1way");


uint32_t nWifi = 1;
uint64_t m_countBeacon = 0;

int64_t offsetError[3];
ApplicationContainer sourceApplications, sinkApplications;

void TxCallback (std::string context, Ptr<const Packet> p)
{
  WifiMacHeader hdr;
  p->PeekHeader (hdr);
  if (hdr.IsBeacon ())
    {
      
      //uint64_t app = (context [10] - '0');
      //NS_LOG_UNCOND (app << "->" << context);
      Time lClock = sourceApplications.Get(0)->GetNode()->GetLocalTime(); // local clock of AP
      
      TimestampTag timestamp;
      timestamp.SetTimestamp(lClock);
      p->AddByteTag(timestamp);

      NS_LOG_UNCOND ("At AP Simulator time: " << Simulator::Now() << " Local time: " << lClock);
      m_countBeacon++;

    }

}

/*********************
void MacTxCallback (std::string context, Ptr<const Packet> p)
{
  
  
  uint64_t app = (context [10] - '0');
  NS_LOG_UNCOND (app << "->" << context);

  Time ts = sourceApplications.Get(0)->GetNode()->GetLocalTime();

  TimestampTag timestamp;
  timestamp.SetTimestamp(ts);
  p->AddByteTag(timestamp);

  NS_LOG_UNCOND (Simulator::Now() << " Local=> " << ts);


}
*****************/

void RxCallback (std::string context, Ptr<const Packet> p)
{

  WifiMacHeader hdr;
  p->PeekHeader (hdr);
  if (hdr.IsBeacon ())
    {
      
      uint app = (context [10] - '0');
      //NS_LOG_UNCOND (app << " => " << context);
      //NS_LOG_UNCOND (Simulator::Now());
      if(app >= nWifi)
      {
        return;
      }

      uint64_t offset;     
      TimestampTag timestamp;

      if (p->FindFirstMatchingByteTag (timestamp))
        {
          
          offset = sinkApplications.Get(app)->GetNode()->GetLocalTime().GetNanoSeconds() - timestamp.GetTimestamp().GetNanoSeconds();
          
        }      

      uint64_t sClock = sinkApplications.Get(app)->GetNode()->GetLocalTime().GetNanoSeconds() - offset;
      uint64_t mClock = sourceApplications.Get(0)->GetNode()->GetLocalTime().GetNanoSeconds();
      offsetError[app] = (int64_t)(offsetError[app] > abs((int64_t)(sClock - mClock))) ? offsetError[app] : sClock - mClock;
      NS_LOG_UNCOND ("Offset Error: " << offsetError[app] << "ns Absolute Error: " <<abs ((int64_t)(sClock - mClock)) << "ns");
    }

}

int main (int argc, char *argv[])
{

  bool enablePcap = true;
  bool enablePcf = true;
  bool withData = true;
  std::string trafficDirection = "downstream";
  uint64_t cfpMaxDurationUs = 7500; //microseconds
  double simulationTime = 4; //seconds

//for (int itr = 10; itr < 100; itr+20)
//{
  CommandLine cmd;
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("enablePcf", "Enable/disable PCF mode", enablePcf);
  cmd.AddValue ("withData", "Enable/disable UDP data packets generation", withData);
  cmd.AddValue ("trafficDirection", "Data traffic direction (if withData is set to 1): upstream (all STAs -> AP) or downstream (AP -> all STAs)", trafficDirection);
  cmd.AddValue ("cfpMaxDuration", "CFP max duration in microseconds", cfpMaxDurationUs);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("enablePcap", "Enable/disable PCAP output", enablePcap);
  cmd.Parse (argc, argv);

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);

  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  WifiMacHelper mac;

  Ssid ssid = Ssid ("wifi-pcf");
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate54Mbps"), "ControlMode", StringValue ("OfdmRate24Mbps"));

  NetDeviceContainer staDevices;
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false),
               "PcfSupported", BooleanValue (enablePcf));
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true),
               "PcfSupported", BooleanValue (enablePcf),
               "CfpMaxDuration", TimeValue (MicroSeconds (cfpMaxDurationUs)),
               "BeaconInterval", TimeValue (MicroSeconds (100000)));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);


  MobilityHelper mobility;

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

  for (uint32_t i = 0; i < nWifi; i++)
    {
      positionAlloc->Add (Vector (30.0, 0.0, 0.0));
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

  
  if (withData)
    {
      uint32_t portNumber = 9;
      for (uint32_t index = 0; index < nWifi; ++index)
        {
          auto ipv4 = (trafficDirection == "upstream") ? wifiApNode.Get (0)->GetObject<Ipv4> () : wifiStaNodes.Get (index)->GetObject<Ipv4> ();

          const auto address = ipv4->GetAddress (1, 0).GetLocal ();
          InetSocketAddress sinkSocket (address, portNumber++);
          OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
          onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
          onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
          onOffHelper.SetAttribute ("DataRate", DataRateValue (50000000 / nWifi));
          onOffHelper.SetAttribute ("PacketSize", UintegerValue (100)); //bytes (default 1472)
          PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
          if (trafficDirection == "upstream")
            {
              sourceApplications.Add (onOffHelper.Install (wifiStaNodes.Get (index)));
              sinkApplications.Add (packetSinkHelper.Install (wifiApNode.Get (0)));
            }
          else if (trafficDirection == "downstream")
            {
              sinkApplications.Add (packetSinkHelper.Install (wifiStaNodes.Get (index)));
              sourceApplications.Add (onOffHelper.Install (wifiApNode.Get (0)));
            }
          else
            {
              NS_ASSERT_MSG (false, "Invalid trafficDirection!");
            }
        }
      sinkApplications.Start (Seconds (0.0));
      sinkApplications.Stop (Seconds (simulationTime + 1));
      sourceApplications.Start (Seconds (1.0));
      sourceApplications.Stop (Seconds (simulationTime + 1));
    }

  //Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::WifiMac/MacTx", MakeCallback (&MacTxCallback));

  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyTxBegin", MakeCallback (&TxCallback));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyRxBegin", MakeCallback (&RxCallback));


  if (enablePcap)
    {
      phy.EnablePcap ("Huawei_Sync_1way", apDevice.Get (0));
    }

  Simulator::Stop (Seconds (simulationTime + 1));
  Simulator::Run ();

  std::cout << "# tx beacons: " << m_countBeacon << std::endl;

//  for (uint j = 0; j < nWifi; ++j)
//    std::cout << offsetError[j];


  Simulator::Destroy ();
  return 0;
}
