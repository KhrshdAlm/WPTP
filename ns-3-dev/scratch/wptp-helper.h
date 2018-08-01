/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#pragma once
#include <stdint.h>
#include <string>
#include "ns3/object-factory.h"
#include "ns3/address.h"
#include "ns3/attribute.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/application-container.h"
#include "wptp-pcf.h"
#include "ns3/onoff-helper.h"

class wptpHelper : public ns3::OnOffHelper
{
public:

  wptpHelper (std::string protocol, Address address);

  int64_t AssignStreams (NodeContainer c, int64_t stream);
  
private:

  Ptr<Application> InstallPriv (Ptr<Node> node) const;

  ObjectFactory m_factory; //!< Object factory.
};