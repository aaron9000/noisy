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

