// Determine browser.

var ie  = document.all && navigator.userAgent.indexOf("Opera")==-1;
var dom = document.getElementById&&!ie&&navigator.userAgent.indexOf("Opera")==-1;

// EOF
