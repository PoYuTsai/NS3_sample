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
#include "stdio.h"
#include <thread>
#include <string>

#include <iostream>
// Default Network Topology
//
// Number of wifi or csma nodes can be increased up to 250
//                          |
//                 Rank 0   |   Rank 1
// -------------------------|----------------------------
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n5   n6   n7   n0 -------------- n1   n2   n3   n4
//                   point-to-point  |    |    |    |
//                                   ================
//                                     LAN 10.1.2.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");


int wifi(int argc, char *argv[],int WIFI_node,int PACKAGE_NUMBER,std::string DELAY_TIME_P,std::string DATA_RATE_P,int Interval,std::string DELAY_TIME_C,std::string DATA_RATE_C){

  printf("node %d package %d allpackage %d \n",WIFI_node,PACKAGE_NUMBER,WIFI_node*PACKAGE_NUMBER*4);
  std::cout <<DATA_RATE_C<<"\n";
  bool verbose = true;
  uint32_t nCsma = 0;
  uint32_t nWifi =WIFI_node;
  bool tracing = false;
  
  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue
  if (nWifi > 250 || nCsma > 250)
    {
      std::cout << "Too many wifi or csma nodes, no more than 250 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue (DATA_RATE_P));
  pointToPoint.SetChannelAttribute ("Delay", StringValue (DELAY_TIME_P));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue (DATA_RATE_C));
  csma.SetChannelAttribute ("Delay", StringValue (DELAY_TIME_C));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);
  NodeContainer wifiApNode = p2pNodes.Get (0);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (10.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
MobilityHelper mobility2;

  mobility2.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (55),
                                 "MinY", DoubleValue (0),
                                 "DeltaX", DoubleValue (1.0),
                                 "DeltaY", DoubleValue (1.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility2.SetMobilityModel ("ns3::RandomWalk2dMobilityModel","Bounds", RectangleValue (Rectangle (-300, 300, 0, 50)));
  mobility2.Install (wifiStaNodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   
   
  mobility.Install (wifiApNode);

  InternetStackHelper stack;
  stack.Install (csmaNodes);
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  address.Assign (staDevices);
  address.Assign (apDevices);
  int port=9;
  UdpEchoServerHelper echoServer (port);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  serverApps.Start (Seconds (1.0));
   

  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), port);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (PACKAGE_NUMBER));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (Interval)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

unsigned int avgirl=0;
  for(avgirl=1;avgirl<=nWifi;avgirl++){
  ApplicationContainer clientApps = echoClient.Install (wifiStaNodes.Get (nWifi - avgirl));
  //ApplicationContainer clientApps2 = echoClient.Install (wifiStaNodes.Get (nWifi-2));

  clientApps.Start (Seconds (2.0));
  //clientApps2.Start (Seconds (2.0));
  }
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (200.0));
  
  
  //output pcap file
  //pointToPoint.EnablePcapAll ("third");
    //  phy.EnablePcap ("log/third", apDevices.Get (0));
   // csma.EnablePcap ("log/third", csmaDevices.Get (0), true);


  Simulator::Run ();
  Simulator::Destroy ();
   printf("end\n");
  return 0;


}
std::string int2str(int &i) {
std::string s;
  std::stringstream ss(s);
  ss << i;

  return ss.str();
}


int 
main (int argc, char *argv[])
{
    
  //std::cout << argv[1];
  //printf ("%c",argv[1][2]);
 // system("script log/av1");
  //mThread.join();
  
  int Interval=0;//s
  int WIFI_node=1;//!!!set how many connect node
  int package=100;//1024byte
  

//WIRE
  std::string delay_time_P="0.1s";//point to point set dalay time  on E-ANDSF database ping driver
  std::string data_rate_P="100Mbps";//point to point set Bandwidth

//CSMA WIRELESS
   std::string delay_time_C="0.1s";
  std::string data_rate_C="100Mbps";
  //try it and enjoy


  for(int in=1;in<400;in++){
    //std::string kbstr;
    //kbstr = int2str(kb)+"Kbps";
    Interval=0.01*in;
   wifi(argc,argv,WIFI_node,package,delay_time_P,data_rate_P,Interval,delay_time_C,data_rate_C);
        
  
   //thread mThread( wifi, WIFI_node );
   
  }
  //mThread.join();
   
   return 0;  

}
