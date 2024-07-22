#ifndef ARPIPELINE_HPP
#define ARPIPELINE_HPP

#include "PatternDetector.hpp"
#include "CameraCalibration.hpp"
#include "GeometryTypes.hpp"
// ARPipeline gestiona la detecci�n de patrones y calcula su pose en el espacio 3D utilizando una imagen de patr�n 
// y una calibraci�n de c�mara.

class ARPipeline
{
public:
  ARPipeline(const cv::Mat& patternImage, const CameraCalibration& calibration);

  bool processFrame(const cv::Mat& inputFrame);

  const Transformation& getPatternLocation() const;

  PatternDetector     m_patternDetector;
private:

private:
  CameraCalibration   m_calibration;
  Pattern             m_pattern;
  PatternTrackingInfo m_patternInfo;
};

#endif