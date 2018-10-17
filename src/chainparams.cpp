// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2017-2018 The Tele developers
// Copyright (c) 2017 The ColossusCoinXT developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "bignum.h"
#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

int64_t fGenTime = 1524509400; //Monday, 23 April 2018 19:50:00 GMT+01:00

void MineGenesis(CBlock genesis) {

	printf("Searching for main net genesis block...\n");
	uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
	uint256 thash;

	while (true)
	{
	thash = genesis.GetHash();
	if (thash <= hashTarget)
	    break;
	if ((genesis.nNonce & 0xFFF) == 0)
	{
	    printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
	}
	++genesis.nNonce;
	if (genesis.nNonce == 0)
	{
	    printf("NONCE WRAPPED, incrementing time\n");
	    ++genesis.nTime;
	}
	}
	printf("genesis.nTime = %u \n", genesis.nTime);
	printf("genesis.nNonce = %u \n", genesis.nNonce);
	printf("genesis.nVersion = %u \n", genesis.nVersion);
	printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str()); //first this, then comment this line out and uncomment the one under.
	printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str()); //improvised. worked for me, to find merkle root


}


/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
	boost::assign::map_list_of
	( 0, uint256("0x00000ec12472cd20da840f87f356e15cdddafea585a445e52b47520795a443bf"));
static const Checkpoints::CCheckpointData data = {
	&mapCheckpoints,
    1524500319, // * UNIX timestamp of last checkpoint block
    101,    // * total number of transactions between genesis and last checkpoint
		    //   (the tx=... number in the SetBestChain debug.log lines)
	2000        // * estimated number of transactions per day after checkpoint
	};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
	boost::assign::map_list_of
	(0, uint256("0x0000040b3b23f7c6d52c9e673f3d324ba46c3f65bc030def40165a5fd3c4aa2a"));

static const Checkpoints::CCheckpointData dataTestnet = {
	&mapCheckpointsTestnet,
	1509477776,
	0,
	250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
	boost::assign::map_list_of
	(0, uint256("0x17f466035feda0210168511310bfea139d434f901e0e200577ffddf1d5c7209a"));

static const Checkpoints::CCheckpointData dataRegtest = {
	&mapCheckpointsRegtest,
	1509477776,
	0,
	100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x91;
        pchMessageStart[1] = 0xc5;
        pchMessageStart[2] = 0xfE;
        pchMessageStart[3] = 0xe0;
        vAlertPubKey = ParseHex("0465b73205eadd3fb3bb2fc7b585d96320d99b56bb605634399f2d4008d3e3261f180bda35ba83a17089284850ff734440ae624fd49960a28a5aca60a51562591f");
        nDefaultPort = 11568;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // tele starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 30;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60;
        nTargetSpacing = 1 * 60;
        nLastPOWBlock = 1000; // Extended PoW phase to help set up the network
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nModifierUpdateBlock = 0;
        nMaxMoneyOut = int64_t(40000000) * COIN;
        nModifierInterval = 60;
        nModifierIntervalRatio = 3;
        //nBudgetPercent = 5;
        nMinStakeAge = 60*60*8; // 8 Hours
        nMasternodeRewardPercent = 60; // 60 % of block reward that goes to masternodes
        nRequiredMasternodeCollateral = 10000 * COIN;

        const char* pszTimestamp = "BBC News 23-April-2018: Stephen Lawrence Day to be held annually";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04727863a84ee21fb97b34c4bb24b019141306567c4637c5b0bae1ea3d08087c43d5ea7d5508dd743f641db6dac904852dc33619cc4613fb17a3639e594e14e765") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = fGenTime;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 7629060;

        hashGenesisBlock = genesis.GetHash();
        
	 //MineGenesis(genesis);

        assert(hashGenesisBlock == uint256("00000ec12472cd20da840f87f356e15cdddafea585a445e52b47520795a443bf"));
        assert(genesis.hashMerkleRoot == uint256("e90d06f549846cde7e04d334f03606d647d037a44c6b8b4ef5d3d346d8694e53"));


        //vFixedSeeds.clear();
        //vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("telecoin.info", "seed1.telecoin.info"));
        vSeeds.push_back(CDNSSeedData("telecoin.info", "seed2.telecoin.info"));
        vSeeds.push_back(CDNSSeedData("95.179.198.11", "95.179.198.11"));
        vSeeds.push_back(CDNSSeedData("95.179.202.39", "95.179.202.39"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 66);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 85);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 153);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(17)(76)(58)(103).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(17)(7)(6)(26).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;

        strSporkPubKey = "04bcba1d11b6f67464bc98f5b800874d5609686156539fa3e7d160de33c76e0927b79c7c10a7c2931d74b200ea22e502151b5a107de2dc2100cc51de19354a9b18";
        strObfuscationPoolDummyAddress = "1HcMTVpwHvcNGoNcsJ2EMyJRHXjZLbr6Wx";
        nStartMasternodePayments = fGenTime;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x93;
        pchMessageStart[1] = 0x8b;
        pchMessageStart[2] = 0x6a;
        pchMessageStart[3] = 0x7e;
        vAlertPubKey = ParseHex("0412dc4793b2ec5602f0b2e36fa0852a4645a2686297bb470727be548f619afbb9c1cef2320540f277530f932e7eaae0816c90ee38363af044ab7b68077d3acaf0");
        nDefaultPort = 21568;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60;
        nTargetSpacing = 1 * 60;
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 0;
        nMaxMoneyOut = 40000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = fGenTime;
        genesis.nNonce = 12465658;

	//MineGenesis(genesis);
        
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0000040b3b23f7c6d52c9e673f3d324ba46c3f65bc030def40165a5fd3c4aa2a"));
        assert(genesis.hashMerkleRoot == uint256("e90d06f549846cde7e04d334f03606d647d037a44c6b8b4ef5d3d346d8694e53"));
        
        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 8); // Testnet tele addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 133);  // Testnet tele script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 127);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
         // Testnet tele BIP32 pubkeys start with 'DRKV'
         base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(177)(60)(206)(172).convert_to_container<std::vector<unsigned char> >();
         // Testnet tele BIP32 prvkeys start with 'DRKP'
         base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(177)(61)(214)(147).convert_to_container<std::vector<unsigned char> >();
         // Testnet tele BIP44 coin type is '1' (All coin's testnet default)
         base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkPubKey = "04bcba1d11b6f67464bc98f5b800874d5609686156539fa3e7d160de33c76e0927b79c7c10a7c2931d74b200ea22e502151b5a107de2dc2100cc51de19354a9b18";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = fGenTime;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nSubsidyHalvingInterval = 150;
	nDefaultPort = 31568;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // tele: 1 day
        nTargetSpacing = 1 * 60;        // tele: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = fGenTime;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 123151;

        
	//MineGenesis(genesis);
	hashGenesisBlock = genesis.GetHash();
	assert(hashGenesisBlock == uint256("17f466035feda0210168511310bfea139d434f901e0e200577ffddf1d5c7209a"));
        assert(genesis.hashMerkleRoot == uint256("e90d06f549846cde7e04d334f03606d647d037a44c6b8b4ef5d3d346d8694e53"));
        
        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
