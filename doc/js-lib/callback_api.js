function Callback( func, target_obj ) {
    this.func       = func;
    this.target_obj = target_obj;
    this.args       = new Array();
    this.add_args( arguments, 2 );
}

Callback.prototype.toString = function() {
    var res = this.target_obj ? 'object.' : '';
    res += JS.funcname( this.func ) + '( ';
    for( var i=0; i < this.args.length; i++ )
        res += this.args[i];
    res += '... )';

    return res;
}

Callback.prototype.add_args = function( args_array, first_arg_index ) {
    var index = first_arg_index ? first_arg_index : 0;
    for( ; index < args_array.length; index++ )
        this.args.push( args_array[index] );
}

Callback.prototype.run = function() {
    if( arguments.length > 0 ) {
        var args_list = new Array();
        for( var i = 0; i < this.args.length; i++ )
            args_list.push( this.args[i] );

        for( var i = 0; i < arguments.length; i++ )
            args_list.push( arguments[i] );
    }
    else
        var args_list = this.args;

    return this.func.apply( this.target_obj, args_list );
}

// EOF
