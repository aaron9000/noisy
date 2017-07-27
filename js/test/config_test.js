import {expect, assert} from 'chai';
import R from 'ramda';
import {parse, DefaultConfig} from '../es5/config';

describe("config", () => {

    it("parses a fully qualified config", () => {
        const config = parse(['node', 'something', '-t', 'perlin_fbm', '-x', '3', '-n', '1', '-l', 'low', '-d', '3', '-i', '32']);
        expect(config).to.deep.equal({
            noise_type: 'perlin_fbm',
            detail_level: 'low',
            image_size: 32,
            dimensions: 3,
            min_points_per_cell: 1,
            max_points_per_cell: 4
        });
    });
    //
    // it("parses a fully qualified config (long version)", () => {
    //     const config = parse(['node', 'something', '--path-a', 'foo1', '--path-b', 'bar1', '--output', 'pretty', '--rows', '101', '--width', '50']);
    //     expect(config).to.deep.equal({
    //         pathA: 'foo1',
    //         pathB: 'bar1',
    //         rows: 101,
    //         output: 'pretty',
    //         width: 50,
    //         buildStringBuffer: false
    //     });
    // });
    //
    // it("parses a minimal config", () => {
    //     const config = parse(['node', 'something', '-a', 'f1', '-b', 'b1']);
    //     expect(config).to.deep.equal({
    //         pathA: 'f1',
    //         pathB: 'b1',
    //         rows: 10,
    //         output: 'pretty',
    //         width: 16,
    //         buildStringBuffer: false
    //     });
    // });

    // it("parses an empty config as default", () => {
    //     const config = parse(['node', 'something', 'asdf']);
    //     expect(config).to.deep.equal(DefaultConfig);
    // });
    //
    // it("parses an invalid config as default", () => {
    //     const config = parse(['node', 'something', '-w', '1000']);
    //     expect(config).to.equal(null);
    // });

});