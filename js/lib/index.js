import {handleConfig} from './app';
import {parse} from './config';

handleConfig(parse(process.argv))
    .then(v => {
        process.exit();
    })
    .catch(err => {
        console.log(err.message);
        console.log(`see 'noisy --help' for usage details`)
    });



const
    spawn = require( 'child_process' ).spawn,
    ls = spawn( 'ls', [ '-lh', '/usr' ] );

ls.stdout.on( 'data', data => {
    console.log( `stdout: ${data}` );
});

ls.stderr.on( 'data', data => {
    console.log( `stderr: ${data}` );
});

ls.on( 'close', code => {
    console.log( `child process exited with code ${code}` );
});