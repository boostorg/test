function Cookie(name, doc) {
    this.$name     = name;
    this.$document = doc ? doc : document;
}

Cookie.prototype.store = function ( expiration, path, domain, secure ) {
    var cookieval = "";
    for( var prop in this ) {
        // Ignore properties with names that begin with '$' and also methods.
        if( (prop.charAt(0) == '$') || ((typeof this[prop]) == 'function') )
            continue;

        if( cookieval != "" )
            cookieval += '&';

        cookieval += prop + ':' + escape(this[prop]);
    }

    var cookie = this.$name + '=' + cookieval;
    if( expiration ) {
        var expiration_time = 0;
        if( expiration.days )
            expiration_time += expiration.days*24*60*60*1000;
        if( expiration.hours )
            expiration_time += expiration.hours*60*60*1000;
        if( expiration.minutes )
            expiration_time += expiration.minutes*60*1000;
        if( expiration.seconds )
            expiration_time += expiration.seconds*1000;

        var exp_date= new Date();
        exp_date.setTime( exp_date.getTime() + expiration_time );
        cookie += '; expires=' + exp_date.toUTCString();
    }
    if( path )   cookie += '; path=' + path;
    if( domain ) cookie += '; domain=' + domain;
    if( secure ) cookie += '; secure';

    this.$document.cookie = cookie;
}

Cookie.prototype.load = function() { 
    var allcookies = this.$document.cookie;

    if( allcookies == "" )
        return false;

    var start = allcookies.indexOf( this.$name + '=' );
    if( start == -1 )
        return false;
    start += this.$name.length + 1;

    var end = allcookies.indexOf( ';', start );
    if( end == -1 )
        end = allcookies.length;
    var cookieval = allcookies.substring(start, end);

    var a = cookieval.split('&');
    for( var i=0; i < a.length; i++ )
        a[i] = a[i].split(':');

    for(var i = 0; i < a.length; i++) {
        this[a[i][0]] = unescape(a[i][1]);
    }

    return true;
}

Cookie.prototype.remove = function( path, domain ) {
    var cookie = this.$name + '=';

    if( path )   cookie += '; path=' + path;
    if( domain ) cookie += '; domain=' + domain;

    cookie += '; expires=Fri, 02-Jan-1970 00:00:00 GMT';

    this.$document.cookie = cookie;
}

// EOF
