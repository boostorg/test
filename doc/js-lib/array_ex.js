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
}

if( !Array.prototype.remove ) {
    Array.prototype.remove = function( el ) {
        var index = this.indexOf( el );

        if( index > -1 ) {
            //!! splice
            for( var index2 = index+1; index2 < this.length; index2++ ) {
                this[index2-1] = this[index2];
            }

            this.pop();
            return true;
        }

        return false;
    };
}

// EOF
