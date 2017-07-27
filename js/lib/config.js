import R from 'ramda';
import t from 'tcomb-validation';
import clear from 'clear-require';
const packageDotJson = require('../package.json');

const SUPPORTED_IMAGE_SIZES = [32, 64, 128, 256, 512 , 1024, 2048];
const MAX_IMAGE_SIZE_FOR_3D = 128;
const WILDCARD_REGEX = /.*/;

export const NoiseType = {
    PERLIN_FBM: "perlin_fbm",
    PERLIN_RIDGE: "perlin_ridge",
    PERLIN_TURBULENCE: "perlin_turbulence",
    WORLEY_DOTS: "worley_dots",
    WORLEY_GEMS: "worley_gems",
    WORLEY_PILLOWS: "worley_pillows",
    WORLEY_RIDGED_PILLOWS: "worley_ridged_pillows",
    WORLEY_CAUSTICS: "worley_caustics"
};
const NoiseTypeType = t.enums(R.invertObj(NoiseType));

export const DetailLevel = {
    LOW: 'low',
    MEDIUM: 'medium',
    HIGH: 'high',
    VERY_HIGH: 'veryhigh'
};
const DetailLevelType = t.enums(R.invertObj(DetailLevel));

const DimensionsType = t.refinement(t.Number, (n) => n == 2 || n == 3, 'expected 2 or 3');
const ImageSizeType = t.refinement(t.Number, (n) => R.contains(n, SUPPORTED_IMAGE_SIZES), 'expected power of 2');
const PointsType = t.refinement(t.Number, (n) => n >= 1 && n <= 10, 'expected 1 - 10');

const Config = t.struct({
    noise_type: NoiseTypeType,
    detail_level: t.maybe(DetailLevelType),
    dimensions: DimensionsType,
    image_size: ImageSizeType,
    min_points_per_cell: t.maybe(PointsType),
    max_points_per_cell: t.maybe(PointsType)
});

export const DefaultConfig = {
    noise_type: null,
    detail_level: DetailLevel.MEDIUM,
    dimensions: 2,
    image_size: 256,
    min_points_per_cell: 1,
    max_points_per_cell: 4
};

function stringEnumeration(enumObject:Object):String {
    return R.pipe(
        R.values,
        v => JSON.stringify(v),
        R.replace(/",/g, ', '),
        R.replace(/"/g, ''),
        R.replace(/\[/g, ''),
        R.replace(/\]/g, '')
    )(enumObject);
}

export function configIsValid(config:Object):Boolean {
    if (config === null){
        return false;
    }
    const {min_points_per_cell, max_points_per_cell, dimensions, image_size} = config;
    const validation = t.validate(config, Config);
    const passesValidation = validation.isValid();
    if (!passesValidation){
        console.log(validation.firstError().message.toLowerCase());
        return false;
    }
    if (min_points_per_cell > max_points_per_cell){
        console.log("min_points is greater than max_points");
        return false;
    }
    if (image_size > MAX_IMAGE_SIZE_FOR_3D && dimensions == 3){
        console.log("image too large for 3D texture " + image_size);
        return false;
    }
    return true;
}

export function parse(args:Array<String>):Object{
    clear('commander');
    const program = require('commander')
        .version(packageDotJson.version.toString())
        .option('-t, --noise_type   <noise_type>', `${stringEnumeration(NoiseType)}`, WILDCARD_REGEX, DefaultConfig.noise_type)
        .option('-l, --detail_level [detail_level]', `${stringEnumeration(DetailLevel)}`, WILDCARD_REGEX, DefaultConfig.detail_level)
        .option('-d, --dimensions   [n]', '2 - 3      *3D not supported for images > 128)', parseInt)
        .option('-i, --image_size   [n]', `32 - 2048  *powers of two only`, parseInt)
        .option('-n, --min_points   [n]', '0 - 10     *worley only', parseInt)
        .option('-x, --max_points   [n]', '1 - 10     *worley only', parseInt)
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

