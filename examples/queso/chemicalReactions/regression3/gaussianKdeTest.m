calib1

scaleFactor=max(queso_calib_0_chain_histBins)/max(queso_calib_0_chain_gaussianKdeDensities);
x=queso_calib_0_chain_kdeEvalPositions;
y=queso_calib_0_chain_gaussianKdeDensities;
exactY=exp(-.5.*x.*x)/sqrt(2*pi);

plot(queso_calib_0_chain_centersOfHistBins(1,1),queso_calib_0_chain_histBins(1,1)/scaleFactor,'b*');
hold;
plot(x(1,1),y(1,1),'r+');
plot(x(1,1),exactY(1,1),'go');

plot(queso_calib_0_chain_centersOfHistBins,queso_calib_0_chain_histBins/scaleFactor,'b*');
plot(x,y,'r+');
plot(x,exactY,'go');

[tmp,n]=size(x);
intY     =sum(y     )*(max(x)-min(x))/n
intExactY=sum(exactY)*(max(x)-min(x))/n

legend('Scaled histogram',...
       'Computed density estimator',...
       'Exact Gaussian distribution',...
       'Location','NorthEast');