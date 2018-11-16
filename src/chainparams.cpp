// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017 The Loonie developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
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
    (0, uint256("0x00000944b26677decf813faf1da74f80e48bc37923d151050d6af6eade4dc5ce"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1541173083, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    200        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

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
        pchMessageStart[0] = 0x7d;
        pchMessageStart[1] = 0xe1;
        pchMessageStart[2] = 0xa0;
        pchMessageStart[3] = 0x6b;
        vAlertPubKey = ParseHex("04737fc14103be1722ab8f81fd63b764efd255aaec57e960e48d99682e46ed99188e995d75361301e76e8e2dd3bcec3904219ebe13e14862059cc718784624d21b");
        nDefaultPort = 6860;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Loonie starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Loonie: 1 minute
        nTargetSpacing = 1 * 60;  // Loonie:  1 minute
        nMaturity = 10;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 130000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 2000;
        nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 90000;
        nAccumulatorStartHeight = 1;
        nBlockEnforceSerialRange = 90003; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint

        const char* pszTimestamp = "Loonie makes a switch to POS/MN - Nov-2018";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04df5cc8133f524f2a70a5902db5bc80b9496a6919b02e4163deb1b10a17e6c81c96b1f77d5275a0dd29a3a072859c6f5327b316c1383b54dc3fbb7a075e28e430") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1541173083;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 3226936;

//                                 if(genesis.GetHash() != uint256("0x"))
// {
//       printf("MSearching for genesis block...\n");
//       uint256 hashTarget;
//       hashTarget.SetCompact(genesis.nBits);
//       while(uint256(genesis.GetHash()) > uint256(hashTarget))
//       {
//           ++genesis.nNonce;
//           if (genesis.nNonce == 0)
//           {
//               printf("Mainnet NONCE WRAPPED, incrementing time");
//               std::cout << std::string("Mainnet NONCE WRAPPED, incrementing time:\n");
//               ++genesis.nTime;
//           }
//           if (genesis.nNonce % 10000 == 0)
//           {
//               printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
//           }
//       }
//       printf("Mainnet block.nTime = %u \n", genesis.nTime);
//       printf("Mainnet block.nNonce = %u \n", genesis.nNonce);
//       printf("Mainnet block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
//       printf("Mainnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
// }

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000944b26677decf813faf1da74f80e48bc37923d151050d6af6eade4dc5ce"));
        assert(genesis.hashMerkleRoot == uint256("0x641b30be5595ccf753c0451888d3c1c1799ce2ea66d4cec1467b9f1759b1a8f2"));

         vSeeds.push_back(CDNSSeedData("195.181.244.223", "195.181.244.223"));
         vSeeds.push_back(CDNSSeedData("40.122.72.200", "40.122.72.200"));
	     vSeeds.push_back(CDNSSeedData("40.68.1.137", "40.68.1.137"));
    
        //vFixedSeeds.clear();
        //vSeeds.clear(); 

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48); //LOONIE Addresses start with L
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 2);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 25);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x16)(0x9b)(0x83)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x0b)(0xcc)(0xe2)(0x2B).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x5d).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04c03a20082e774c6c93da0251e00ed00e73b28d2f3bbb692b7ab7ec64a8a7b9374c16afa15b6d7355fc482b5aa6b8fc7ec4d0fb71fcc21bdd9b07bf06a7c9d611";
        strObfuscationPoolDummyAddress = "LNk2Ay36rGPFGXq1KkULMpzF4d7PBdjGH6";
        nStartMasternodePayments = 1541173083;

        /** Zerocoin */
        zerocoinModulus = "0xc95577b6dce0049b0a27c779af38079355abadde1a1d80c353f6cb697a7ae5a087bad39caa5798478551d0f9d91e6267716506f32412de1d19d17588765eb9502b85c6a18abdb05791cfd8b734e960281193705eeece210920cc922b3af3ceb178bf12c22eb565d5767fbf19545639be8953c2c38ffad41f3371e4aac750ac2d7bd614b3faabb453081d5d88fdbb803657a980bc93707e4b14233a2358c97763bf28f7c933206071477e8b371f229bc9ce7d6ef0ed7163aa5dfe13bc15f7816348b328fa2c1e69d5c88f7b94cee7829d56d1842d77d7bb8692e9fc7b7db059836500de8d57eb43c345feb58671503b932829112941367996b03871300f25efb5";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * ZCENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 2;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
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
         pchMessageStart[0] = 0xbb;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0x39;
        pchMessageStart[3] = 0x88;
        vAlertPubKey = ParseHex("041b2b4c86273359acac3522471911ed2b303eaab65e8a1de01c06e89f2eab1e55234a4b504f3ce20c6f661f007d0ca15623b4358d9855c7c8ba793a24fa315e22");
        nDefaultPort = 14125;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Loonie: 1 day
        nTargetSpacing = 1 * 15;  // Loonie: 15 seconds
        nLastPOWBlock = 200;
        nMaturity = 2;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 39999999999 * COIN;
        nZerocoinStartHeight = 3;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1515616140;
        genesis.nNonce = 1288918;

        bnProofOfWorkLimit = ~uint256(0) >> 12;

//          if(genesis.GetHash() != uint256("0x"))
// {
//       printf("MSearching for genesis block...\n");
//       uint256 hashTarget;
//       hashTarget.SetCompact(genesis.nBits);
//       while(uint256(genesis.GetHash()) > uint256(hashTarget))
//       {
//           ++genesis.nNonce;
//           if (genesis.nNonce == 0)
//           {
//               printf("Maintest NONCE WRAPPED, incrementing time");
//               std::cout << std::string("Maintest NONCE WRAPPED, incrementing time:\n");
//               ++genesis.nTime;
//           }
//           if (genesis.nNonce % 10000 == 0)
//           {
//               printf("Maintest: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
//           }
//       }
//       printf("testnet block.nTime = %u \n", genesis.nTime);
//       printf("testnet block.nNonce = %u \n", genesis.nNonce);
//       printf("testnet block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
//       printf("testnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
// }

  	hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000030328e931620f5182c49ce0ae3b30de723ccd3ef1a71605df2e13c6a640"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // vSeeds.push_back(CDNSSeedData("45.77.38.21", "45.77.38.21"));
        // vSeeds.push_back(CDNSSeedData("45.77.40.11", "45.77.40.11"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48); // Testnet loonie addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 2);  // Testnet loonie script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 25);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet loonie BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x22)(0x6a)(0x61)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet loonie BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x22)(0x80)(0x48)(0x4d).convert_to_container<std::vector<unsigned char> >();
        // Testnet loonie BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0xbc).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04abb5e65280dda6a113fadfb9877f9c399532245fe1acb61de293ab298034d5084277fab3768774a3b68cbbe5021cc5049ec8c9997a13f64da1afa0bcfb156db1";
        strObfuscationPoolDummyAddress = "Ceax8jHDQ1s2kHVjysEoTQncVdUrNBuXtp";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
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
        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xd6;
        pchMessageStart[2] = 0x33;
        pchMessageStart[3] = 0x3c;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Loonie: 1 day
        nTargetSpacing = 1 * 60;        // Loonie: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1515524400;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 5834482;

//                                 if(genesis.GetHash() != uint256("0x"))
// {
//       printf("MSearching for genesis block...\n");
//       uint256 hashTarget;
//       hashTarget.SetCompact(genesis.nBits);
//       while(uint256(genesis.GetHash()) > uint256(hashTarget))
//       {
//           ++genesis.nNonce;
//           if (genesis.nNonce == 0)
//           {
//               printf("REGnet NONCE WRAPPED, incrementing time");
//               std::cout << std::string("REGnet NONCE WRAPPED, incrementing time:\n");
//               ++genesis.nTime;
//           }
//           if (genesis.nNonce % 10000 == 0)
//           {
//               printf("REGnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
//           }
//       }
//       printf("reg block.nTime = %u \n", genesis.nTime);
//       printf("reg block.nNonce = %u \n", genesis.nNonce);
//       printf("reg block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
//       printf("reg block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
// }

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 14325;
        assert(hashGenesisBlock == uint256("0x00000887ba6f0ff5a68e0d6b7267ea5acc939082a6afc6caa528a45d2d9983c6"));

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
        nDefaultPort = 14425;
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

static CChainParams* pCurrentParams = 0;

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
