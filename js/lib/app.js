import R from 'ramda';
import Promise from 'bluebird';
import {configIsValid} from './config';

const spawn = require('child_process').spawn;

export function handleConfig(config:Object):Promise {
    // console.log("debug config", config);
    if (!configIsValid(config)) return Promise.reject(new Error(''));
    

    return new Promise((resolve, reject) => {
        const ls = spawn('ls', ['-lh', '/usr']);
        ls.stdout.on('data', data => {
            console.log(`stdout: ${data}`);
        });
        ls.stderr.on('data', data => {
            console.log(`stderr: ${data}`);
        });
        ls.on('close', code => {
            console.log(`child process exited with code ${code}`);
            resolve('done');
        });
    });
}



