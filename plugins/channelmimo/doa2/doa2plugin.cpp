///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2022 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include "doa2plugin.h"

#include <QtPlugin>
#include "plugin/pluginapi.h"

#ifndef SERVER_MODE
#include "doa2gui.h"
#endif
#include "doa2.h"
#include "doa2webapiadapter.h"
#include "doa2plugin.h"

const PluginDescriptor DOA2Plugin::m_pluginDescriptor = {
    DOA2::m_channelId,
    QStringLiteral("DOA 2 sources"),
    QStringLiteral("7.3.1"),
    QStringLiteral("(c) Edouard Griffiths, F4EXB"),
    QStringLiteral("https://github.com/f4exb/sdrangel"),
    true,
    QStringLiteral("https://github.com/f4exb/sdrangel")
};

DOA2Plugin::DOA2Plugin(QObject* parent) :
    QObject(parent),
    m_pluginAPI(0)
{
}

const PluginDescriptor& DOA2Plugin::getPluginDescriptor() const
{
    return m_pluginDescriptor;
}

void DOA2Plugin::initPlugin(PluginAPI* pluginAPI)
{
    m_pluginAPI = pluginAPI;

    // register channel MIMO
    m_pluginAPI->registerMIMOChannel(DOA2::m_channelIdURI, DOA2::m_channelId, this);
}

void DOA2Plugin::createMIMOChannel(DeviceAPI *deviceAPI, MIMOChannel **bs, ChannelAPI **cs) const
{
	if (bs || cs)
	{
		DOA2 *instance = new DOA2(deviceAPI);

		if (bs) {
			*bs = instance;
		}

		if (cs) {
			*cs = instance;
		}
	}
}

#ifdef SERVER_MODE
ChannelGUI* DOA2Plugin::createMIMOChannelGUI(
        DeviceUISet *deviceUISet,
        MIMOChannel *mimoChannel) const
{
    (void) deviceUISet;
    (void) mimoChannel;
    return nullptr;
}
#else
ChannelGUI* DOA2Plugin::createMIMOChannelGUI(DeviceUISet *deviceUISet, MIMOChannel *mimoChannel) const
{
    return DOA2GUI::create(m_pluginAPI, deviceUISet, mimoChannel);
}
#endif

ChannelWebAPIAdapter* DOA2Plugin::createChannelWebAPIAdapter() const
{
	return new DOA2WebAPIAdapter();
}