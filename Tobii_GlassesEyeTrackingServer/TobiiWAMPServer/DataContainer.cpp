#include "stdafx.h"
#include "DataContainer.h"


bool DataContainer::instanceFlag = false;
DataContainer* DataContainer::instance = NULL;

DataContainer* DataContainer::getInstance()
{
	if (!instanceFlag)
	{
		instance = new DataContainer();
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

void DataContainer::addGazeSample(ptree sample)
{
	bool result = cGazeSamples.push(sample);
	if (!result) { //Failed to add sample because container is full

	}
}

bool DataContainer::getGazeSample(ptree &OutSample)
{
	return cGazeSamples.pop(OutSample);
}

//void DataContainer::addSceneCameraSample(TobiiEyeSample imageSample)
//{
//	bool result = cSceneImageSamples.push(imageSample);
//	if (!result) { //Failed to add image because container is full
//
//	}
//}

//bool DataContainer::getSceneCameraSample(iViewDataStreamSceneImage &OutImageSample)
//{
//	return cSceneImageSamples.pop(OutImageSample);
//}

//void DataContainer::addEncodedSceneCameraSample(iViewDataStreamH264SceneImage imageSample)
//{
//	bool result = cEncodedSceneImageSamples.push(imageSample);
//	if (!result) { //Failed to add image because container is full
//
//	}
//}
//
//bool DataContainer::getEncodedSceneCameraSample(iViewDataStreamH264SceneImage &OutImageSample)
//{
//	return cEncodedSceneImageSamples.pop(OutImageSample);
//}

//VP encoded samples
//void DataContainer::addVPEncodedSceneCameraSample(EncodedVPXSample imageSample) {
//	bool result = cVPCustomEncodedSceneCameraSamples.push(imageSample);
//	if (!result) {
//		//delete imageSample;
//	}
//}
//
//bool DataContainer::getVPEncodedSceneCameraSample(EncodedVPXSample &OutImageSample) {
//	return cVPCustomEncodedSceneCameraSamples.pop(OutImageSample);
//}
