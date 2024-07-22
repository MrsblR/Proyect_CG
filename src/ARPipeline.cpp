
#include "ARPipeline.hpp"
// Constructor que inicializa el detector de patrones con una imagen de patr�n y una calibraci�n de c�mara.
ARPipeline::ARPipeline(const cv::Mat& patternImage, const CameraCalibration& calibration)
  : m_calibration(calibration)
{
  m_patternDetector.buildPatternFromImage(patternImage, m_pattern);
  m_patternDetector.train(m_pattern);
}
// Procesa un frame de entrada para detectar el patr�n y calcular su pose si es encontrado.

bool ARPipeline::processFrame(const cv::Mat& inputFrame)
{
  bool patternFound = m_patternDetector.findPattern(inputFrame, m_patternInfo);

  if (patternFound)
  {
    m_patternInfo.computePose(m_pattern, m_calibration);
  }

  return patternFound;
}
// Devuelve la transformaci�n que representa la ubicaci�n del patr�n en el espacio 3D.

const Transformation& ARPipeline::getPatternLocation() const
{
  return m_patternInfo.pose3d;
}