import R from 'ramda';
import Promise from 'bluebird';
import {configIsValid} from './config';

const exec = require('child_process').exec;

export function handleConfig(config:Object):Promise {
    if (!configIsValid(config)) return Promise.reject(new Error(''));
    const cmd = __dirname + '/../bin/' + config.command;
    return new Promise((resolve, reject) => {
        exec(cmd, (error, stdout, stderr) => {
            if (error) {
                console.error(`exec error: ${error}`);
                reject(error);
            }else{
                console.log(stdout);
                if (stderr){
                    console.log(`lib error: ${stderr}`);
                }else{
                    console.log('success');
                    resolve({success: true});
                }
            }

        });
    });
}



