#pragma once

#include <ostream>

namespace ecs {
	using compId_type = uint8_t;
	enum compID : compId_type {


		_LAST_COMP_ID
	};
	constexpr compId_type maxComponentID = _LAST_COMP_ID;


	using hdlrId_type = uint8_t;
	enum hdlrID : hdlrId_type {



		_LAST_HDLR_ID
	};
	constexpr hdlrId_type maxHandlerID = _LAST_HDLR_ID;


	using grpId_type = uint8_t;
	enum grpID : grpId_type {
		_grp_GENERAL = 0,

		_LAST_GRP_ID
	};
	constexpr grpId_type maxGroupID = _LAST_GRP_ID;


	using sysId_type = uint8_t;
	enum sysId : sysId_type {


		_LAST_SYS_ID
	};
	constexpr sysId_type maxSystemId = _LAST_SYS_ID;


	using msgId_type = uint8_t;
	enum msgId : msgId_type {


	};


	struct Message {
		msgId_type id;

	};


}


