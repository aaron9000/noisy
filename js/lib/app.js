import R from 'ramda';
import Promise from 'bluebird';

import {parse, configIsValid} from './config';

export function handleConfig(config:Object):Promise {
    if (!configIsValid(config)) return Promise.reject(new Error(`invalid config`));
    return config.pathB ? diff(config) : render(config);
}