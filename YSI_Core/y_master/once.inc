/*
Legal:
	Version: MPL 1.1
	
	The contents of this file are subject to the Mozilla Public License Version 
	1.1 the "License"; you may not use this file except in compliance with 
	the License. You may obtain a copy of the License at 
	http://www.mozilla.org/MPL/
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the YSI framework.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright C 2011
	the Initial Developer. All Rights Reserved.

Contributors:
	Y_Less
	koolk
	JoeBullet/Google63
	g_aSlice/Slice
	Misiur
	samphunter
	tianmeta
	maddinat0r
	spacemud
	Crayder
	Dayvison
	Ahmad45123
	Zeex
	irinel1996
	Yiin-
	Chaprnks
	Konstantinos
	Masterchen09
	Southclaws
	PatchwerkQWER
	m0k1
	paulommu
	udan111

Thanks:
	JoeBullet/Google63 - Handy arbitrary ASM jump code using SCTRL.
	ZeeX - Very productive conversations.
	koolk - IsPlayerinAreaEx code.
	TheAlpha - Danish translation.
	breadfish - German translation.
	Fireburn - Dutch translation.
	yom - French translation.
	50p - Polish translation.
	Zamaroht - Spanish translation.
	Los - Portuguese translation.
	Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes for
		me to strive to better.
	Pixels^ - Running XScripters where the idea was born.
	Matite - Pestering me to release it and using it.

Very special thanks to:
	Thiadmer - PAWN, whose limits continue to amaze me!
	Kye/Kalcor - SA:MP.
	SA:MP Team past, present and future - SA:MP.

Optional plugins:
	Gamer_Z - GPS.
	Incognito - Streamer.
	Me - sscanf2, fixes2, Whirlpool.
*/

#define Master_ID() (_@)

// These parts should only be defined once ever.  This code is not dependent on
// the current MASTER value, it's all generic.

#define _YSIM_COMPARE -1

#define MASTER_DATA<%0> static stock Bit:YSI_g_sMasterData[_:(%0)];
#define MASTER_RESET<%0> (YSI_g_sMasterData[_:(%0)] = Bit:0);
#define MASTER_SET<%0> (YSI_g_sMasterData[_:(%0)] = Bit:((1) << (Master_Caller())));
#define MASTER_ADD<%0> (YSI_g_sMasterData[_:(%0)] |= Bit:(1 << Master_Caller()));
#define MASTER_REMOVE<%0> (YSI_g_sMasterData[_:(%0)] &= Bit:(~(1 << Master_Caller())));
#define MASTER_EMPTY<%0> if(!YSI_g_sMasterData[_:(%0)])
#define MASTER_EXCLUSIVE<%0> (YSI_g_sMasterData[_:(%0)] == Bit:(1 << Master_ID()))
#define MASTER_GET<%0> (YSI_g_sMasterData[_:(%0)])
#define MASTER_COPY<%0,%1> (YSI_g_sMasterData[_:(%0)] = YSI_g_sMasterData[_:(%1)]);

#if defined YSI_IS_CLIENT
	#if NO_VALUE(YSI_IS_CLIENT)
		#undef YSI_IS_CLIENT
		#define YSI_IS_CLIENT 100
	#endif
#endif

#if defined YSI_IS_SERVER
	#if NO_VALUE(YSI_IS_SERVER)
		#undef YSI_IS_SERVER
		#define YSI_IS_SERVER 100
	#endif
#endif

#if defined YSI_IS_STUB
	#if NO_VALUE(YSI_IS_STUB)
		#undef YSI_IS_STUB
		#define YSI_IS_STUB 100
	#endif
#endif

#define mhook master_hook
#define mtask master_task
#define mptask master_ptask

// These are the macros used by y_master for the recursive function definition
// structure, but they are not used unless a cloud-based "foreign" or "global"
// are used.

#define W@(@Zu:#%0:%1#,%2);return%3(%8) W@(@Zu:#%1,%2);return%0:%3(%0:%8)
#define Z@(%0string:i) S@(i),Q@
#define V@(%0string:%1) F@(),Q@


// "foreign" declaration.
#define @Zu:#%0,_YM@C%8:,,);return%1(%2);} @Z%8:#%0,#);return%1(%2);}


// Second "foreign" declaration.
// This has been updated to check for arrays, then check if the array is a
// string, instead of the old method which checked for those separately.
#define @Zn:@Zo:%2:#%0#%1|||%3[%4]|||%5,%6);%7} @Zm:@Zr:%2:#%0#%1|||%3|||%5,%6);%7}

#define @Zm:@Zr:%2:#%0#%1|||%4string:%3|||%5,%6);%7} @Zn:@Zo:%2:#%0s#%1,%3|||%5|||%6);%7}
#define @Zr:%2:#%0#%1|||%3|||%5,%6);%7} @Zn:@Zo:%2:#%0a#%1,%3|||%5|||%6);%7}

#define @Zo:%2:#%0#%1|||%3|||%5,%6);%7} @Zn:@Zo:%2:#%0i#%1,%3|||%5|||%6);%7}

#define @Zp:%0||||||);%1} @Zl:%0);%1}
#define @Zy:%0||||||);%1} @Zj:%0);%1}

#define _YM@Cl:%0,%1);%2} @Zn:@Zo:@Zp:##|||%0|||%1);%2}
#define _YM@Cj:%0,%1);%2} @Zn:@Zo:@Zy:##|||%0|||%1);%2}

#define @Zl:%0);%8void:%3(%1);} %0);}
#define @Zj:%0);%8void:%3(%1);} %0);%3(%1);}

// "global" declaration.
#define @Zt:@Zb:@Zh:@Zi:%0(%1|||%2:%3|||%5,%6) @Zb:@Zh:@Zi:%0(%1|||%3|||%5,%6)
#define @Zb:@Zh:@Zi:%0(%1|||%3[%4]|||%5,%6) @Zt:@Zb:@Zh:@Zi:%0(%1,%3|||%5|||%6)
#define @Zh:@Zi:%0(%1|||%3|||%5,%6) @Zt:@Zb:@Zh:@Zi:%0(%1,%3|||%5|||%6)

#define @Zi:%0(,%1||||||) %0(%1)

#define @Zk:_YM@CP:%0(,,) %0()

#define _YM@CP:%0(%1,%2) @Zt:@Zb:@Zh:@Zi:%0(|||%1|||%2)

#define X@(@Zk:_YM@CP:%0string:%1(%2)) R@(@Zk:_YM@CP:%1(%2))

// This is nearly all I needed to add to "global" for "void:".  I doubt that
// "foreign" will be as simple!
#define @Zz:X@(@Zk:_YM@CP:%0void:%1(%2)) @Zk:_YM@CP:%1(%2)

#define YSIM_NOT_CLIENT (!YSIM_HAS_MASTER || !_YSIM_IS_CLIENT)

#define @global global

// Set up the master ID stack.
#define _YSIM_PUSH_INDEX 0 // First position on the stack to push to.

#if !defined MAX_MASTERS
	#define MAX_MASTERS 64 // Default, old value.
#endif

#define HANDOFF(%9) master_func MAKE_YCM<HANDOFF_SOURCE...>()<p>

forward Master_Reassert();

enum _E_YCM
{
	_E_YCM@y,
	_E_YCM@n,
	_E_YCM@m,
	_E_YCM@p,
	_E_YCM@u
}

#define _YCM@y (_YCM@==_E_YCM@y)
#define _YCM@n (_YCM@==_E_YCM@n)
#define _YCM@m (_YCM@==_E_YCM@m)
#define _YCM@p (_YCM@==_E_YCM@p)
#define _YCM@u (_YCM@==_E_YCM@u)

#if defined YSI_NO_MASTER
	#endinput
#endif

static
	YSI_g_sMasterCount,
	YSI_g_sMasterData[MAX_MASTERS];

PRE_HOOK(Master)
#undef CHAIN_ORDER
#define CHAIN_ORDER @CO_Master

/*-------------------------------------------------------------------------*//**
 * <returns>
 *  Next master ID to be assigned.
 * </returns>
 *//*------------------------------------------------------------------------**/

stock Master_GetNext()
{
	P:3("Master_GetNext called");
	new
		masters = getproperty(8, YSIM_MASTER),
		i = 0;
	while (i != 32)
	{
		if (!(masters & (1 << i)))
		{
			return i;
		}
		++i;
	}
	return -1;
}

/*-------------------------------------------------------------------------*//**
 * <returns>
 *  OnScriptInit
 * </returns>
 * <remarks>
 *  Constructor.  Gets the script a master ID.  Now ALWAYS gets an ID, even if
 *  the master system is disabled - doing otherwise is just too complicated.
 * </remarks>
 * <transition keep="true" target="_ALS : _ALS_go" />
 *//*------------------------------------------------------------------------**/

public OnScriptInit()
{
	P:1("Master_OnScriptInit");
	state _ALS : _ALS_go;
	if (!existproperty(8, YSIM_MASTER))
	{
		setproperty(8, YSIM_MASTER, 1);
	}
	// Properties get lost between script changes so we need to force a rebuild.
	CallRemoteFunction("Master_Reassert", "");
	new
		masters = getproperty(8, YSIM_MASTER),
		i = 1;
	while (i != 32)
	{
		if (!(masters & (1 << i)))
		{
			_@ = i;
			masters |= 1 << i;
			break;
		}
		++i;
	}
	if (i != 32)
	{
		setproperty(8, YSIM_MASTER, masters);
	}
	C:1(if (!_@) P:E("MasterID not assigned"););
	// Make sure this is called before all other YSI initialisations, at least
	// all the ones which use the master system.
	CallLocalFunction("YSIM_OnMasterSystemInit", "");
	Master_OnScriptInit();
	return 1;
}

HOOK_FORWARD:Master_OnScriptInit();
#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
	#define _ALS_OnScriptInit
#endif
#define OnScriptInit(%0) Master_OnScriptInit(%0) <_ALS : _ALS_go>

#define OnMasterSystemInit YSIM_OnMasterSystemInit
forward YSIM_OnMasterSystemInit();

/*-------------------------------------------------------------------------*//**
 * <returns>
 *  OnGameModeExit
 * </returns>
 * <remarks>
 *  Destructor.
 * </remarks>
 *//*------------------------------------------------------------------------**/

public OnScriptExit()
{
	P:1("MasterOnce_OnScriptExit called");
	// Loop through everything this script is master for and call the remote
	// function for it.  EXCEPT for this script itself!
	new
		func[4];
	for (new i = 0, j = YSI_g_sMasterCount; i != j; ++i)
	{
		// This is slightly slower for ending and starting scripts, but uses far
		// less heap space, and these values are rarely used, so may as well
		// pack them (which is what has happened here).
		func[0] = YSI_g_sMasterData[i] & 0xFF,
		func[1] = (YSI_g_sMasterData[i] >> 8) & 0xFF,
		func[2] = YSI_g_sMasterData[i] >> 16,
		CallLocalFunction(func, "");
		P:5("Master_OnScriptExit: %d %d %s", i, YSI_g_sMasterCount, func);
		// The properties currently clear instantly, but that may not always be
		// the case.
	}
	setproperty(8, YSIM_MASTER, getproperty(8, YSIM_MASTER) & ~(1 << _@)),
	Master_OnScriptExit(),
	CallRemoteFunction("OnMasterSystemClose", "i", _@);
	return 1;
}

HOOK_FORWARD:Master_OnScriptExit();
#if defined _ALS_OnScriptExit
	#undef OnScriptExit
#else
	#define _ALS_OnScriptExit
#endif
#define OnScriptExit(%0) Master_OnScriptExit(%0) <_ALS : _ALS_go>

//#define OnMasterSystemClose Master_OnScriptClose
//forward Master_OnScriptClose(id);

/*-------------------------------------------------------------------------*//**
 * <remarks>
 *  Rebuilds the collection of master data whenever a script is restarted.
 * </remarks>
 *//*------------------------------------------------------------------------**/

public Master_Reassert()
{
	// Make sure that the caller parameter is always 0 by default.
	U@(8, YSIM_CALLER, 0);
	if (_@)
	{
		// Read this script's master value.
		setproperty(8, YSIM_MASTER, getproperty(8, YSIM_MASTER) | (1 << _@) | 1);
		// Readd this script's owned scripts.
		new
			func[4];
		for (new i = 0; i != YSI_g_sMasterCount; ++i)
		{
			// This is slightly slower for ending and starting scripts, but uses far
			// less heap space, and these values are rarely used, so may as well
			// pack them (which is what has happened here).
			func[0] = YSI_g_sMasterData[i] & 0xFF;
			func[1] = (YSI_g_sMasterData[i] >> 8) & 0xFF;
			func[2] = YSI_g_sMasterData[i] >> 16;
			setproperty(9, func, _@);
		}
	}
}

stock _Master_Relinquish(library[])
{
	//printf("STEAL %s %d", library, YSI_g_sMasterCount);
	switch (YSI_g_sMasterCount)
	{
		case 0: return;
		case 1: YSI_g_sMasterCount = 0;
		default:
		{
			new
				m = library[0] | (library[1] << 8) | (library[2] << 16);
			// Last one, don't switch.
			if (YSI_g_sMasterData[--YSI_g_sMasterCount] == m) return;
			// Find where this master is in the list and remove it.
			for (new i = 0; i != YSI_g_sMasterCount; ++i)
			{
				if (YSI_g_sMasterData[i] == m)
				{
					// Shuffle them to same space.
					YSI_g_sMasterData[i] = YSI_g_sMasterData[YSI_g_sMasterCount];
					return;
				}
			}
		}
	}
}

/*-------------------------------------------------------------------------*//**
 * <param name="library">The name of the library to try become master for.</param>
 *//*------------------------------------------------------------------------**/

stock bool:_Master_Get(library[], bool:force = false)
{
	new
		bool:ret = true;
	P:3("bool:_Master_Get called: \"%s\", %i", library, _:force);
	P:2("_Master_Get called");
	if (existproperty(9, library))
	{
		new
			master = getproperty(9, library);
		P:4("_Master_Get: Prop exists: %d %d", master, _@);
		if (master != -1)
		{
			P:4("_Master_Get: Prop set");
			if (master == _@) return true; // Nothing has changed.
			else if (force) ret = false; // A server is forcing itself.
			else return false; // Not got the master.
		}
	}
	P:4("_Master_Get: Get master.");
	setproperty(9, library, _@);
	P:4("_Master_Get: Set master.");
	// Add this library to the list.  The list is designed to only deal with
	// two or three character master names now!
	if (YSI_g_sMasterCount < MAX_MASTERS)
	{
		P:4("_Master_Get: Set master string %s = %d", library, library[0] | (library[1] << 8) | (library[2] << 16));
		YSI_g_sMasterData[YSI_g_sMasterCount++] = library[0] | (library[1] << 8) | (library[2] << 16);
		P:4("_Master_Get: Set master string.");
	}
	P:C(else P:E("Too many master scripts"););
	return ret;
}

/*----------------------------------------------------------------------------*\
	Forward all the 64 possible master script initialisation functions.
	
	The code is split up every 26 masters to slightly speed up the compilation
	by using "#endinput" if "MAX_MASTERS" is low enough.
\*----------------------------------------------------------------------------*/

#if defined @aOnScriptInit
	forward @aOnScriptInit();
#endif
#if defined @bOnScriptInit
	forward @bOnScriptInit();
#endif
#if defined @cOnScriptInit
	forward @cOnScriptInit();
#endif
#if defined @dOnScriptInit
	forward @dOnScriptInit();
#endif
#if defined @eOnScriptInit
	forward @eOnScriptInit();
#endif
#if defined @fOnScriptInit
	forward @fOnScriptInit();
#endif
#if defined @gOnScriptInit
	forward @gOnScriptInit();
#endif
#if defined @hOnScriptInit
	forward @hOnScriptInit();
#endif
#if defined @iOnScriptInit
	forward @iOnScriptInit();
#endif
#if defined @jOnScriptInit
	forward @jOnScriptInit();
#endif
#if defined @kOnScriptInit
	forward @kOnScriptInit();
#endif
#if defined @lOnScriptInit
	forward @lOnScriptInit();
#endif
#if defined @mOnScriptInit
	forward @mOnScriptInit();
#endif
#if defined @nOnScriptInit
	forward @nOnScriptInit();
#endif
#if defined @oOnScriptInit
	forward @oOnScriptInit();
#endif
#if defined @pOnScriptInit
	forward @pOnScriptInit();
#endif
#if defined @qOnScriptInit
	forward @qOnScriptInit();
#endif
#if defined @rOnScriptInit
	forward @rOnScriptInit();
#endif
#if defined @sOnScriptInit
	forward @sOnScriptInit();
#endif
#if defined @tOnScriptInit
	forward @tOnScriptInit();
#endif
#if defined @uOnScriptInit
	forward @uOnScriptInit();
#endif
#if defined @vOnScriptInit
	forward @vOnScriptInit();
#endif
#if defined @wOnScriptInit
	forward @wOnScriptInit();
#endif
#if defined @xOnScriptInit
	forward @xOnScriptInit();
#endif
#if defined @yOnScriptInit
	forward @yOnScriptInit();
#endif
#if defined @zOnScriptInit
	forward @zOnScriptInit();
#endif

#if MAX_MASTERS <= 26
	#endinput
#endif

#if defined @AOnScriptInit
	forward @AOnScriptInit();
#endif
#if defined @BOnScriptInit
	forward @BOnScriptInit();
#endif
#if defined @COnScriptInit
	forward @COnScriptInit();
#endif
#if defined @DOnScriptInit
	forward @DOnScriptInit();
#endif
#if defined @EOnScriptInit
	forward @EOnScriptInit();
#endif
#if defined @FOnScriptInit
	forward @FOnScriptInit();
#endif
#if defined @GOnScriptInit
	forward @GOnScriptInit();
#endif
#if defined @HOnScriptInit
	forward @HOnScriptInit();
#endif
#if defined @IOnScriptInit
	forward @IOnScriptInit();
#endif
#if defined @JOnScriptInit
	forward @JOnScriptInit();
#endif
#if defined @KOnScriptInit
	forward @KOnScriptInit();
#endif
#if defined @LOnScriptInit
	forward @LOnScriptInit();
#endif
#if defined @MOnScriptInit
	forward @MOnScriptInit();
#endif
#if defined @NOnScriptInit
	forward @NOnScriptInit();
#endif
#if defined @OOnScriptInit
	forward @OOnScriptInit();
#endif
#if defined @POnScriptInit
	forward @POnScriptInit();
#endif
#if defined @QOnScriptInit
	forward @QOnScriptInit();
#endif
#if defined @ROnScriptInit
	forward @ROnScriptInit();
#endif
#if defined @SOnScriptInit
	forward @SOnScriptInit();
#endif
#if defined @TOnScriptInit
	forward @TOnScriptInit();
#endif
#if defined @UOnScriptInit
	forward @UOnScriptInit();
#endif
#if defined @VOnScriptInit
	forward @VOnScriptInit();
#endif
#if defined @WOnScriptInit
	forward @WOnScriptInit();
#endif
#if defined @XOnScriptInit
	forward @XOnScriptInit();
#endif
#if defined @YOnScriptInit
	forward @YOnScriptInit();
#endif
#if defined @ZOnScriptInit
	forward @ZOnScriptInit();
#endif

#if MAX_MASTERS <= 52
	#endinput
#endif

// We can't permute these symbol names - a symbol can't start with a number.
#if defined @0OnScriptInit
	forward @0OnScriptInit();
#endif
#if defined @1OnScriptInit
	forward @1OnScriptInit();
#endif
#if defined @2OnScriptInit
	forward @2OnScriptInit();
#endif
#if defined @3OnScriptInit
	forward @3OnScriptInit();
#endif
#if defined @4OnScriptInit
	forward @4OnScriptInit();
#endif
#if defined @5OnScriptInit
	forward @5OnScriptInit();
#endif
#if defined @6OnScriptInit
	forward @6OnScriptInit();
#endif
#if defined @7OnScriptInit
	forward @7OnScriptInit();
#endif
#if defined @8OnScriptInit
	forward @8OnScriptInit();
#endif
#if defined @9OnScriptInit
	forward @9OnScriptInit();
#endif

#if defined @@OnScriptInit
	forward @@OnScriptInit();
#endif

#if defined @_OnScriptInit
	forward @_OnScriptInit();
#endif

