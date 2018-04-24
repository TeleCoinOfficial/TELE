// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The Tele Team developers
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
	( 0, uint256("0x00000176d99ba593e22957abe48c081d3b94c7bfc26b9448e1c7c7fc075315b2"));
static const Checkpoints::CCheckpointData data = {
	&mapCheckpoints,
    1524500319, // * UNIX timestamp of last checkpoint block
    101,    // * total number of transactions between genesis and last checkpoint
		    //   (the tx=... number in the SetBestChain debug.log lines)
	2000        // * estimated number of transactions per day after checkpoint
	};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
	boost::assign::map_list_of
	(0, uint256("0x00000d49336370af05a0b3dd7bf16a4bf580a4e2311178a75377b1813f86b6d0"));

static const Checkpoints::CCheckpointData dataTestnet = {
	&mapCheckpointsTestnet,
	1509477776,
	0,
	250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
	boost::assign::map_list_of
	(0, uint256("0x6915a5ff8705e85a24f07f9498dd950a550e20f590f7b3f46f04b3fe47080381"));

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
        vAlertPubKey = ParseHex("0000098d3ba6ba6e7423fa5cbd6a89e0a9a5348f88d332b44a5cb1a8b7ed2c1eaa335fc8dc4f012cb8241cc0bdafd6ca70c5f5448916e4e6f511bcd746ed57dc50");
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
        nLastPOWBlock = 10000; // Extended PoW phase to help set up the network
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nModifierUpdateBlock = 0;
        nMaxMoneyOut = int64_t(40000000) * COIN;
        nModifierInterval = 60;
        nModifierIntervalRatio = 3;
        //nBudgetPercent = 5;
        nMinStakeAge = 60*60*8; // 8 Hours
        nMasternodeRewardPercent = 60; // % of block reward that goes to masternodes
        nRequiredMasternodeCollateral = 5000 * COIN;

        const char* pszTimestamp = "BBC News 23-April-2018: Stephen Lawrence Day to be held annually";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 5654 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04fbc0c407fb2e6d3b18f686f49f19fc58a3790b1770188d6512b2cf8cd534ba194dddbecd52e582abb8a119638aad6ca1ae31f4646a0ef2715119ab70320d4c13") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = fGenTime;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 6048180;

        hashGenesisBlock = genesis.GetHash();
        
	// MineGenesis(genesis);

        assert(hashGenesisBlock == uint256("00000176d99ba593e22957abe48c081d3b94c7bfc26b9448e1c7c7fc075315b2"));
        assert(genesis.hashMerkleRoot == uint256("8a78a52f1a46e0bba46e2009c75ef14f6436f2f30c6640c0c3d91aec7a1c0ccf"));


        //vFixedSeeds.clear();
        //vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("107.191.45.64", "107.191.45.64")); 
		//vSeeds.push_back(CDNSSeedData("144.202.65.252", "144.202.65.252"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 0x17);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 0x76);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 0xff);
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
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;

        strSporkKey = "04B433E6598390C992F4F022F20D3B4CBBE691652EE7C48243B81701CBDB7CC7D7BF0EE09E154E6FCBF2043D65AF4E9E97B89B5DBAF830D83B9B7F469A6C45A717";
        strObfuscationPoolDummyAddress = "D87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
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
        vAlertPubKey = ParseHex("04cd195b65e689d6b3d68b921c62c1f6cac549c82926236f57faa60b3dfbd714f7b378c5eb8e2ae1ed89e6e81eba4af4232703e26af8b275449ec00fbba0a1201b");
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
        genesis.nNonce = 9093903;

	//MineGenesis(genesis);
        
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("00000d49336370af05a0b3dd7bf16a4bf580a4e2311178a75377b1813f86b6d0"));
        assert(genesis.hashMerkleRoot == uint256("8a78a52f1a46e0bba46e2009c75ef14f6436f2f30c6640c0c3d91aec7a1c0ccf"));
        
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
        strSporkKey = "04348C2F50F90267E64FACC65BFDC9D0EB147D090872FB97ABAE92E9A36E6CA60983E28E741F8E7277B11A7479B626AC115BA31463AC48178A5075C5A9319D4A38";
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
        genesis.nNonce = 12350;

        
	//MineGenesis(genesis);
	hashGenesisBlock = genesis.GetHash();
	assert(hashGenesisBlock == uint256("6915a5ff8705e85a24f07f9498dd950a550e20f590f7b3f46f04b3fe47080381"));
        assert(genesis.hashMerkleRoot == uint256("8a78a52f1a46e0bba46e2009c75ef14f6436f2f30c6640c0c3d91aec7a1c0ccf"));
        
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
