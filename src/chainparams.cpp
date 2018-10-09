// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The SMRT developers
// Copyright (c) 2018 The NodeLife developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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
    (0, uint256("0x000004bbdad4fc22b6503801e3e815f3ebbc2be025e2533d7498d2062159d27a"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1524823200, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x00000d66e266ab435906c004707fc71d2b03a43ca4ea02b5cf5d042c3aae4963"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1524823201,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x69206017867b5c2a969ac77f17d913e90577486b5253497419b2a808dbcf17da"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1524823202,
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
        pchMessageStart[0] = 0x63;
        pchMessageStart[1] = 0x99;
        pchMessageStart[2] = 0xaa;
        pchMessageStart[3] = 0xb9;
        vAlertPubKey = ParseHex("04a192fb2af9dc52817816fed3b3b923dd2670e4c28636f7d24b30d21f3f98c97dbc1fd401987854e8cecfc3eac29c3d3c70f3150cccc72a7b94f0165efe681974");
        nDefaultPort = 52310;
        bnProofOfWorkLimit = ~uint256(0) >> 10; // NodeLife starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 65; // NLF: 65s
        nTargetSpacing = 1 * 65;  // NLF: 65sec
		nLastPOWBlock = 200;
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nModifierUpdateBlock = 615800;
        nMaxMoneyOut = 8000000 * COIN;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "NodeLife for life";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 130 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("042c0bb78097f0e75f2e7d0ada0a638e1cccbd3c30469a22726ee96dd63e9fcca8b09fa6ec7d52c786b544c68db3201ddfcaf04f088c501c9985d6efa88b62c0f2") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1524823200;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 0x4ce800;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000004bbdad4fc22b6503801e3e815f3ebbc2be025e2533d7498d2062159d27a"));
        assert(genesis.hashMerkleRoot == uint256("0x4d99c2d971a19d4235bf756f694d550d7ecc4158acfc7a9d665b745d9eb2c101"));

        vSeeds.push_back(CDNSSeedData("nodelife.io", "seed.nodelife.io"));     // Primary DNS Seeder 
        vSeeds.push_back(CDNSSeedData("nodelife.io", "seed2.nodelife.io"));    // Secondary DNS Seeder

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 63); //K
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19); //7
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x27)(0x34).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x36)(0x2C).convert_to_container<std::vector<unsigned char> >();
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
        strSporkKey = "0489dab9fa1d91da3f35e3f6584a8c47df1cb970494b6a3a8bb803a7ecf698be01b751d5767544bee40da4b89dfae476d521921b83f5fc7fcd4993839154e28ff9";
        strObfuscationPoolDummyAddress = "Sj6vTuj5DJoJqLzA44wJd6FyTBgGf3x4TZ";
        nStartMasternodePayments = 1524826800; 
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
        pchMessageStart[0] = 0x35;
        pchMessageStart[1] = 0x54;
        pchMessageStart[2] = 0x45;
        pchMessageStart[3] = 0xca;
        vAlertPubKey = ParseHex("04c47bdb0470445cee5749919b8731503e5fd75715a1be282a822b7fc8e440cecb35feff0d1db76489d66397f07919edb2dc372f07621cefcb19c1298740b794a2");
        nDefaultPort = 52312;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 65; // NLF: 65
        nTargetSpacing = 1 * 65;  // NLF: 65 sec
        nLastPOWBlock = 2;
        nMaturity = 11;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1524823201;
        genesis.nNonce = 0xd27a04;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000d66e266ab435906c004707fc71d2b03a43ca4ea02b5cf5d042c3aae4963"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("nodelife.io", "testnet.seed.nodelife.io"));
        vSeeds.push_back(CDNSSeedData("nodelife.io", "testnet.seed2.nodelife.io"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 125); // s
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 21);  // 8
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet smrt BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet smrt BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet smrt BIP44 coin type is '1' (All coin's testnet default)
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
        strSporkKey = "049644048f62b87b43b797510c41837992181cf99644d930f4b35c41f26b4948b7111a01674e53ae8642c30beafafd147ac25d27a665a32c16df29beb128c36316";
        strObfuscationPoolDummyAddress = "sMnxYNsF9X2PVYpUj8rDwSSpXiRipeQ2MU";
        nStartMasternodePayments = 1524826801; //
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
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // NLF: 1 day
        nTargetSpacing = 1 * 65;        // NLF: 45 sec
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1524823200;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 0x3041;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 52314;
        assert(hashGenesisBlock == uint256("0x69206017867b5c2a969ac77f17d913e90577486b5253497419b2a808dbcf17da"));

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
        nDefaultPort = 52320;
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
