if( !Array.prototype.push ) {
    Array.prototype.push = function( new_el ) {
        this[this.length] = new_el;

        return this.length;
    };
}

if( !Array.prototype.pop ) {
    Array.prototype.pop = function() {
        if( this.length > 0 ) {
            var last_element = this[this.length-1];
            this.length = this.length-1;

            return last_element;
        }
        else
            return null;
    };
}

if( !Array.prototype.indexOf ) {
    Array.prototype.indexOf = function( el ) {
        for( var index = 0; index < this.length; ++index ) {
            if( this[index] == el )
                return index;
        }

        return -1;
    }
}

if( !Array.prototype.contains ) {
    Array.prototype.contains = function( el ) {
        return this.indexOf( el ) != -1;
};

if( !Array.prototype.remove ) {
    Array.prototype.remove = function( el ) {
    var index = this.indexOf( el );

    if( index > -1 ) {
        for( var index2 = index+1; index2 < this.length; index2++ ) {
            this[index2-1] = this[index2];
        }

        this.pop();
        return true;
    }

    return false;
};

// EOF
