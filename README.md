# PMTGainCalibration

 The basic idea is that we do not need to know the full spectral shape of the PMT SPE response. All we need to know for calibration is the mean and variance of the SPE response. This is because when you have several photoelectrons in your signal, the linearity of the PMT ensures that the response converges to a Gaussian distribution by the Central Limit Theorem (see figure below). 
