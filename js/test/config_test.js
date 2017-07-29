import {expect, assert} from 'chai';
import R from 'ramda';
import {parse, DefaultConfig} from '../es5/config';

describe("config", () => {

    it("parses a fully qualified config", () => {
        const config = parse([
            'node', 'something',
            '-t', 'perlin_fbm',
            '-x', '3',
            '-n', '2',
            '-l', 'low',
            '-d', '3',
            '-i', '32'
        ]);
        expect(config).to.deep.equal({
            noise_type: 'perlin_fbm',
            detail_level: 'low',
            image_size: 32,
            dimensions: 3,
            min_points_per_cell: 2,
            max_points_per_cell: 3
        });
    });

    it("parses a fully qualified config (long version)", () => {
        const config = parse([
            'node', 'something',
            '--noise_type', 'perlin_fbm',
            '--detail_level', 'low',
            '--dimensions', '3',
            '--image_size', '64',
            '--min_points_per_cell', '2',
            '--max_points_per_cell', '6'
        ]);
        expect(config).to.deep.equal({
            noise_type: 'perlin_fbm',
            detail_level: 'low',
            image_size: 64,
            dimensions: 3,
            min_points_per_cell: 2,
            max_points_per_cell: 6
        });
    });

    it("parses a minimal config", () => {
        const config = parse([
            'node', 'something',
            '-t', 'perlin_fbm',
            '-l', 'low'
        ]);
        expect(config).to.deep.equal({
            noise_type: 'perlin_fbm',
            detail_level: 'low',
            image_size: DefaultConfig.image_size,
            dimensions: DefaultConfig.dimensions,
            min_points_per_cell: DefaultConfig.min_points_per_cell,
            max_points_per_cell: DefaultConfig.max_points_per_cell
        });
    });

    it("parses an invalid configs as null", () => {

        const n = (a, b) => {
            return R.concat(['node', 'something', '-t', 'perlin_fbm', '-l', 'low'], [a, b]);
        };
        expect(parse(['node', 'something', '-t', 'foo'])).to.equal(null);
        expect(parse(['node', 'something', '-t', 'perlin_fbm', '-l', 'foo'])).to.equal(null);
        expect(parse(n('-x', 23))).to.equal(null);
        expect(parse(n('-n', 33))).to.equal(null);
        expect(parse(n('-d', 32))).to.equal(null);
        expect(parse(n('-i', 41))).to.equal(null);
    });

});