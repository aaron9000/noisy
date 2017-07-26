import R from 'ramda';
import t from 'tcomb-validation';
import clear from 'clear-require';

const packageDotJson = require('../package.json');

export const DEFAULT_COLUMN_WIDTH = 16;
export const OutputFormat = {
    PRETTY: 'pretty',
    CSV: 'csv',
    MD: 'md',
    NONE: 'none'
};
const OutputFormatEnum = t.enums(R.invertObj(OutputFormat));
const OUTPUT_FORMAT_REGEX = /^(pretty|csv|md|none)$/i;
const Rows = t.refinement(t.Number, (n) => n % 1 === 0 && n >= 0, 'Integer');

const Config = t.struct({
    pathA: t.maybe(t.String),
    pathB: t.maybe(t.String),
    rows: Rows,
    output: OutputFormatEnum,
    width: ColumnWidth,
    buildStringBuffer: t.maybe(t.Bool)
});

export const DefaultConfig = {
    pathA: null,
    pathB: null,
    rows: 10,
    output: 'pretty',
    width: 16,
    buildStringBuffer: false

};

function stringEnumeration(enumObject:Object):String {
    return R.pipe(
        R.values,
        v => JSON.stringify(v),
        R.replace(/",/g, ', '),
        R.replace(/"/g, '')
    )(enumObject);
}

export function configIsValid(config:Object):Boolean {
    return t.validate(config, Config).isValid();
}

export function parse(args:Array<String>):Object{
    clear('commander');
    const program = require('commander')
        .version(packageDotJson.version.toString())
        .option('-a, --path-a <path>', 'source csv path')
        .option('-b, --path-b <path>', 'comparison csv path (for diff)')
        .option('-r, --rows [n]', 'max rows to output (0 for unlimited)', parseInt)
        .option('-o, --output [output]', `output format ${stringEnumeration(OutputFormat)}`, OUTPUT_FORMAT_REGEX, DefaultConfig.format)
        .option('-w, --width [n]', `max width for pretty & md columns [${COLUMN_MIN_WIDTH} - ${COLUMN_MAX_WIDTH}]`, parseInt)
        .parse(args);
    const keys = R.keys(Config.meta.props);
    return R.pipe(
        R.pickAll(keys),
        R.reduce((accum, value) => {
            return R.isNil(accum[value]) ? R.assoc(value, DefaultConfig[value], accum) : accum;
        }, R.__, keys),
        c => configIsValid(c) ? c : null
    )(program);
}

