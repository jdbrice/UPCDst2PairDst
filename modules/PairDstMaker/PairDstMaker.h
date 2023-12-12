#ifndef PAIR_DST_MAKER_H
#define PAIR_DST_MAKER_H


#include "TreeAnalyzer.h"
#include "XmlRange.h"

#include "vendor/loguru.h"

#include "TNamed.h"
#include "TTree.h"

#include <map>


#include "FemtoDstFormat/BranchReader.h"
#include "FemtoDstFormat/BranchWriter.h"
#include "FemtoDstFormat/TClonesArrayReader.h"
#include "FemtoDstFormat/FemtoEvent.h"
#include "FemtoDstFormat/FemtoTrack.h"
#include "FemtoDstFormat/FemtoBTofPidTraits.h"
#include "FemtoDstFormat/FemtoTrackProxy.h"

#include "PairDstFormat/FemtoPair.h"

#include "upcdst/StUPCEvent.h"
#include "upcdst/StUPCTrack.h"
#include "upcdst/StUPCVertex.h"


template <>
TString XmlConfig::get<TString>( string path ) const {
	TString r( getString( path ) );
	return r;
}

template <>
TString XmlConfig::get<TString>( string path, TString dv ) const {
	if ( !exists( path ) )
		return dv;
	TString r( getString( path ) );
	return r;
}

class PairDstMaker : public TreeAnalyzer
{
protected:

	BranchReader<StUPCEvent> _fer;
	// TClonesArrayReader<FemtoTrack> _ftr;
	// TClonesArrayReader<FemtoBTofPidTraits> _fbtofr;


	TTree * _pairDst = nullptr;
	BranchWriter<FemtoPair> _fpw;
	FemtoPair _pair; 

	double daughterMass = 0.0005109989461;

public:

	const int DEBUG = 1;
	PairDstMaker() {}
	~PairDstMaker() {}

	std::map<string, TH1*> histogram;

	virtual void initialize(){
		TreeAnalyzer::initialize();

		this->_fer.setup( this->chain, "mUPCEvent" );
		// this->_ftr.setup( this->chain, "Tracks" );
		// this->_fbtofr.setup( this->chain, "BTofPidTraits" );

		book->cd();

		this->_pairDst = new TTree( "PairDst", "" );
		this->_fpw.createBranch( this->_pairDst, "Pairs", 256000, 99 );

		daughterMass = config.get<double>( "daughterMass", 0.0005109989461 );

		LOG_F( INFO, "Daughter Mass = %f", daughterMass );

		histogram[ "hZDCEast" ] = new TH1F( "hZDCEast", "", 4095, 0, 4095 );
		histogram[ "hZDCWest" ] = new TH1F( "hZDCWest", "", 4095, 0, 4095 );
		histogram[ "hZDCEastWest" ] = new TH2F( "hZDCEastWest", ";East;West", 4095, 0, 4095, 4095, 0, 4095 );
		
		histogram[ "hNumGlobal" ] = new TH1F( "hNumGlobal", "Num Global", 250, 0, 250 );
		histogram[ "hNumPrimary" ] = new TH1F( "hNumPrimary", "Num Primary", 250, 0, 250 );
		histogram[ "hNumVertex" ] = new TH1F( "hNVertex", "Num vertex", 25, 0, 25 );
		histogram[ "hVertexId" ] = new TH1F( "hVertexId", "vertex ID", 25, 0, 25 );
		histogram[ "hDCAXY" ] = new TH1F( "hDCAXY", "DCA XY", 500, 0, 5 );
		histogram[ "hDCAZ" ] = new TH1F( "hDCAZ", "DCA Z", 500, 0, 15 );

		histogram[ "hVertexX" ] = new TH1F( "hVertexX", "Vertex X", 500, -5, 5 );
		histogram[ "hVertexY" ] = new TH1F( "hVertexY", "Vertex Y", 500, -5, 5 );
		histogram[ "hVertexZ" ] = new TH1F( "hVertexZ", "Vertex Z", 300, -150, 150 );

		histogram[ "sigptmass" ] = new TH2F( "sigptmass", ";pt;mass", 3000, 0, 3.0, 600, 0, 2.0 );
		histogram[ "ptmass" ] = new TH2F( "ptmass", ";pt;mass", 3000, 0, 3.0, 600, 0, 2.0 );
		histogram[ "lsptmass" ] = new TH2F( "lsptmass", "ls;pt;mass", 3000, 0, 3.0, 600, 0, 2.0 );

		histogram[ "t" ] = new TH1F( "t", ";t;mass", 3000, 0, 0.5 );
		histogram[ "lst" ] = new TH1F( "lst", ";t;mass", 3000, 0, 0.5 );
		histogram[ "sigt" ] = new TH1F( "sigt", ";t;mass", 3000, 0, 0.5 );

		histogram[ "hPionPID" ] = new TH1F( "hPionPID", ";t;mass", 500, 0, 100 );

	}
protected:

	// void addPair(FemtoEvent * event, FemtoTrackProxy &p1, FemtoTrackProxy &p2){
		
	// 	if ( p1._track->mId == p2._track->mId) return;

	// 	this->_pair.reset();
	// 	this->_pair.mVertexZ              = event->mPrimaryVertex_mX3;
	// 	this->_pair.mDeltaVertexZ         = event->mPrimaryVertex_mX3 - event->mVpdVz;
	// 	this->_pair.mGRefMult             = this->_ftr.N();//event->mGRefMult;

	// 	this->_pair.mZDCEast             = event->mZDCEast;
	// 	this->_pair.mZDCWest             = event->mZDCWest;


	// 	this->_pair.d1_mPt                = p1._track->mPt;
	// 	this->_pair.d1_mEta               = p1._track->mEta;
	// 	this->_pair.d1_mPhi               = p1._track->mPhi;
	// 	this->_pair.d1_mgPt               = p1._track->mgPt;
	// 	this->_pair.d1_mgEta              = p1._track->mgEta;
	// 	this->_pair.d1_mgPhi              = p1._track->mgPhi;

	// 	this->_pair.d1_mId                = p1._track->mId;
	// 	this->_pair.d1_mNHitsFit          = p1._track->mNHitsFit;
	// 	this->_pair.d1_mNHitsMax          = p1._track->mNHitsMax;
	// 	this->_pair.d1_mNHitsDedx         = p1._track->mNHitsDedx;
	// 	this->_pair.d1_mNSigmaPion        = p1._track->mNSigmaPion;
	// 	this->_pair.d1_mNSigmaKaon        = p1._track->mNSigmaKaon;
	// 	this->_pair.d1_mNSigmaProton      = p1._track->mNSigmaProton;
	// 	this->_pair.d1_mNSigmaElectron    = p1._track->mNSigmaElectron;
	// 	this->_pair.d1_mDCA               = p1._track->mDCA;
	// 	// this->_pair.d1_mPid               = p1._pid;

	// 	if ( nullptr != p1._btofPid ){
	// 		this->_pair.d1_mYLocal               = p1._btofPid->mBTofYLocal;
	// 		this->_pair.d1_mZLocal               = p1._btofPid->mBTofZLocal;
	// 		this->_pair.d1_mTof                  = p1._btofPid->mBTof;
	// 		this->_pair.d1_mLength               = p1._btofPid->mLength;
	// 		this->_pair.d1_mMatchFlag            = p1._btofPid->mBTofMatchFlag;
	// 	} else {
	// 		this->_pair.d1_mYLocal               = -999;
	// 		this->_pair.d1_mZLocal               = -999;
	// 		this->_pair.d1_mTof                  = 0;
	// 		this->_pair.d1_mLength               = 0;
	// 		this->_pair.d1_mMatchFlag            = 0;
	// 	}


	// 	this->_pair.d2_mPt                = p2._track->mPt;
	// 	this->_pair.d2_mEta               = p2._track->mEta;
	// 	this->_pair.d2_mPhi               = p2._track->mPhi;
	// 	this->_pair.d2_mgPt                = p2._track->mgPt;
	// 	this->_pair.d2_mgEta               = p2._track->mgEta;
	// 	this->_pair.d2_mgPhi               = p2._track->mgPhi;

	// 	this->_pair.d2_mId                = p2._track->mId;
	// 	this->_pair.d2_mNHitsFit          = p2._track->mNHitsFit;
	// 	this->_pair.d2_mNHitsMax          = p2._track->mNHitsMax;
	// 	this->_pair.d2_mNHitsDedx         = p2._track->mNHitsDedx;
	// 	this->_pair.d2_mNSigmaPion        = p2._track->mNSigmaPion;
	// 	this->_pair.d2_mNSigmaKaon        = p2._track->mNSigmaKaon;
	// 	this->_pair.d2_mNSigmaProton      = p2._track->mNSigmaProton;
	// 	this->_pair.d2_mNSigmaElectron    = p2._track->mNSigmaElectron;
	// 	this->_pair.d2_mDCA               = p2._track->mDCA;
	// 	// this->_pair.d2_mPid               = p2._pid;

	// 	if ( nullptr != p2._btofPid ){
	// 		this->_pair.d2_mYLocal               = p2._btofPid->mBTofYLocal;
	// 		this->_pair.d2_mZLocal               = p2._btofPid->mBTofZLocal;
	// 		this->_pair.d2_mTof                  = p2._btofPid->mBTof;
	// 		this->_pair.d2_mLength               = p2._btofPid->mLength;
	// 		this->_pair.d2_mMatchFlag            = p2._btofPid->mBTofMatchFlag;
	// 	} else {
	// 		this->_pair.d2_mYLocal               = -999;
	// 		this->_pair.d2_mZLocal               = -999;
	// 		this->_pair.d2_mTof                  = 0;
	// 		this->_pair.d2_mLength               = 0;
	// 		this->_pair.d2_mMatchFlag            = 0;
	// 	}

	// 	TLorentzVector lv1, lv2, lv;

	// 	// // lv = p1._track->lv( 0.1056583745 ) + p2._track->lv( 0.1056583745 ); 
	// 	lv = p1._track->lv( daughterMass ) + p2._track->lv( daughterMass ); 

	// 	// // if ( lv.M() < 0.22 ){
	// 	// // 	LOG_F(INFO, "track1.pt, eta, phi = %f, %f, %f", p1._track->mPt, p1._track->mEta, p1._track->mPhi );
	// 	// // 	LOG_F(INFO, "track2.pt, eta, phi = %f, %f, %f", p2._track->mPt, p2._track->mEta, p2._track->mPhi );
	// 	// // }
		
	// 	this->_pair.mChargeSum = p1._track->mQ + p2._track->mQ;
	// 	this->_pair.mMass      = lv.M();
	// 	this->_pair.mPt        = lv.Pt();
	// 	this->_pair.mEta       = lv.PseudoRapidity();
	// 	this->_pair.mPhi       = lv.Phi();
	// 	this->_pair.mRapidity  = lv.Rapidity();

	// 	this->_fpw.set( this->_pair );
	// 	this->_pairDst->Fill();

	// }



	virtual void analyzeEvent(){
		// LOG_F( INFO, "%s", chain->GetFile()->GetName() );
		// FemtoEvent * event = this->_fer.get();
		StUPCEvent *ev = this->_fer.get();

		this->_pair.reset();
		// getNumberOfVertices()
		// this->_pair.mVertexZ              = event->mPrimaryVertex_mX3;
		// this->_pair.mDeltaVertexZ         = event->mPrimaryVertex_mX3 - event->mVpdVz;
		this->_pair.mGRefMult             = ev->getNGlobTracks();

		this->_pair.mZDCEast             = ev->getZDCUnAttEast();
		this->_pair.mZDCWest             = ev->getZDCUnAttWest();

		histogram[ "hZDCEast" ]->Fill( ev->getZDCUnAttEast() );
		histogram[ "hZDCWest" ]->Fill( ev->getZDCUnAttWest() );
		histogram[ "hZDCEastWest" ]->Fill( ev->getZDCUnAttEast(), ev->getZDCUnAttWest() );
		histogram[ "hNumGlobal" ]->Fill( ev->getNGlobTracks() );
		histogram[ "hNumPrimary" ]->Fill( ev->getNPrimTracks() );
		histogram[ "hNumVertex" ]->Fill( ev->getNPrimVertices() );
		for ( int iv = 0; iv < ev->getNPrimVertices(); iv++ ){
			if ( ev->getVertexId(iv) == nullptr ) continue;

			histogram[ "hVertexX" ]->Fill( ev->getVertexId(iv)->getPosX() );
			histogram[ "hVertexY" ]->Fill( ev->getVertexId(iv)->getPosY() );
			histogram[ "hVertexZ" ]->Fill( ev->getVertexId(iv)->getPosZ() );
		}

		if (ev->getZDCUnAttEast() < 30 || ev->getZDCUnAttWest() < 30) return;
		if (ev->getZDCUnAttEast() > 200 || ev->getZDCUnAttWest() > 200) return;
		if ( ev->getNPrimVertices() > 1 ) return;

		int nPairs = 0;
		for ( size_t i = 0; i < ev->getNumberOfTracks(); i++){
			StUPCTrack * t1 =  ev->getTrack(i);
			TLorentzVector lv1;
			TLorentzVector lv2;
			TLorentzVector lv;
			t1->getLorentzVector( lv1, daughterMass );
			histogram[ "hVertexId" ]->Fill( t1->getVertexId() );

			histogram["hDCAXY"]->Fill( t1->getDcaXY() );
			histogram["hDCAZ"]->Fill( t1->getDcaZ() );
				
			for ( size_t j = i; j < ev->getNumberOfTracks(); j++){
				if ( i == j ) continue;
				StUPCTrack * t2 = ev->getTrack(j);
				if ( t1->getVertexId() != t2->getVertexId() )
					continue;
				t2->getLorentzVector( lv2, daughterMass );

				this->_pair.d1_mPt                = lv1.Pt();
				this->_pair.d1_mEta               = lv1.Eta();
				this->_pair.d1_mPhi               = lv1.Phi();

				this->_pair.d1_mNSigmaPion        = t1->getNSigmasTPCPion();
				this->_pair.d1_mNSigmaKaon        = t1->getNSigmasTPCKaon();
				this->_pair.d1_mNSigmaProton      = t1->getNSigmasTPCProton();
				this->_pair.d1_mNSigmaElectron    = t1->getNSigmasTPCElectron();
				this->_pair.d1_mDCA               = t1->getDcaXY();
				this->_pair.d1_mTof 			  = t1->getTofTime();
				this->_pair.d1_mLength 			  = t1->getTofPathLength();

				this->_pair.d2_mPt                = lv2.Pt();
				this->_pair.d2_mEta               = lv2.Eta();
				this->_pair.d2_mPhi               = lv2.Phi();

				this->_pair.d2_mNSigmaPion        = t2->getNSigmasTPCPion();
				this->_pair.d2_mNSigmaKaon        = t2->getNSigmasTPCKaon();
				this->_pair.d2_mNSigmaProton      = t2->getNSigmasTPCProton();
				this->_pair.d2_mNSigmaElectron    = t2->getNSigmasTPCElectron();
				this->_pair.d2_mDCA               = t2->getDcaXY();
				this->_pair.d2_mTof 			  = t2->getTofTime();
				this->_pair.d2_mLength 			  = t2->getTofPathLength();

				lv = lv1 + lv2;

				this->_pair.mPt                = lv.Pt();
				this->_pair.mEta               = lv.Eta();
				this->_pair.mPhi               = lv.Phi();
				this->_pair.mRapidity 				= lv.Rapidity();
				this->_pair.mMass      = lv.M();
				this->_pair.mChargeSum = t1->getCharge() + t2->getCharge();

				this->_fpw.set( this->_pair );
				this->_pairDst->Fill();

				nPairs++;

				double pionPID = ( pow( t1->getNSigmasTPCPion(), 2 ) + pow( t2->getNSigmasTPCPion(), 2 ) );
				histogram["hPionPID"]->Fill( pionPID );
				if ( pionPID < 25 && fabs(t1->getDcaXY()) < 1.0 && fabs( t2->getDcaXY() ) < 1.0 ){
					if ( (t1->getCharge() + t2->getCharge()) == 0 ){
						histogram[ "ptmass" ]->Fill( lv.Pt(), lv.M() );
						if (lv.M() > 0.6 && lv.M() < 0.95)
							histogram["t"]->Fill( lv.Pt()*lv.Pt() );
					} else {
						histogram[ "lsptmass" ]->Fill( lv.Pt(), lv.M() );
						if (lv.M() > 0.6 && lv.M() < 0.95)
							histogram["lst"]->Fill( lv.Pt()*lv.Pt() );
					}
				}

			}
		}

		// cout << "nPairs = " << nPairs << endl;
		// cout << "ev->"
		
	}

	virtual void postMake(){
		TreeAnalyzer::postMake();

		this->_pairDst->Write();

		histogram[ "sig" ] = (TH1*)histogram["ptmass"]->Clone("sig");
		histogram[ "sig" ]->Add( histogram["lsptmass"], -1 );

		histogram[ "sigt" ] = (TH1*)histogram["t"]->Clone("sigt");
		histogram[ "sigt" ]->Add( histogram["lst"], -1 );

		// for ( auto kv : histogram ){
		// 	kv.second->Write();
		// }


	}

};


#endif