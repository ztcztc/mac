function r = ratioa(c) 
 
% The function RATIOA calculates the local non-orthogonal leakage for pair of adjoining components,
% where r is calculated according to the following rule
% being applied to the case when c is the 2-D array.
%	and k is the number of IMF components.
%
% Calling sequence-
% r=ratioa(c)
%
% Input-
%	c	- 2-D matrix c(n,k) of IMF components
%		 excluding the trend component.
% Output-
%	r	- vector r(k) of the ratio for adjoining components
%		- r(i) = | (c(1,i)*c(1,j)+...+c(n,i)*c(n,j)) | /
%

%----- Get dimensions and initialize

[npt,knb] = size(c);
r1=zeros(knb-1,1);

%----- Calculate the ratio for each pair
for i=1:knb-1,
   j=i+1;
   a1=abs(sum(c(:,i).*c(:,j)));
   b1=sum(c(:,i).*c(:,i))+sum(c(:,j).*c(:,j));
   r1(i)=a1/b1;
end
%r=mean(r1);
%s=std(r1);
%q=max(r1-r);
r=r1;