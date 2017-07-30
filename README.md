# noisy

Generate procedural noise from from the command line.


### Features

- Supports Perlin and Worley noise
- Can generate 2D and 3D noise
- Seemless tiling in all dimensions
- Easy to use

### Noise Types
##### worley_pillows
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/worley_pillows.gif)

##### worley_ridged_pillows
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/worley_ridged_pillows.gif)

##### worley_caustics
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/worley_caustics.gif)

##### worley_gems
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/worley_gems.gif)

##### worley_dots
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/worley_dots.gif)

##### perlin_fbm
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/perlin_fbm.gif)

##### perlin_ridge
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/perlin_ridge.gif)

##### perlin_turbulence
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/perlin_turbulence.gif)


### Installation
```
npm install -g noisy
```


### Recipes

###### Generate 2D perlin noise
```noisy -t perlin_fbm -l medium -d 2```

###### Generate 2D worley noise 
```noisy -t worley_pillows -l medium -d 2```

###### Generate 3D worley noise
```noisy -t worley_gems -l medium -d 3 -i 32```

### Argument Reference

|                                               long |                                              short |                                               type |                                        description |                                            default |
|----------------------------------------------------|----------------------------------------------------|----------------------------------------------------|----------------------------------------------------|----------------------------------------------------|
| `                                    -—noise_type` | `                                              -t` | `                                          string` | `                         see "Noise Types" above` | `                                      perlin_fbm` |
| `                                  -—detail_level` | `                                              -l` | `                                          string` | `  amount of detail (low, medium, high, veryhigh)` | `                                          medium` |
| `                                    -—dimensions` | `                                              -d` | `                                         integer` | `               2 or 3 (3D for image_size <= 128)` | `                                               2` |
| `                                    -—image_size` | `                                              -i` | `                                         integer` | `     powers of two between 32 - 2048 (inclusive)` | `                                             256` |
| `                           -—min_points_per_cell` | `                                              -n` | `                                         integer` | `                      0 - 10 (worley noise only)` | `                                               1` |
| `                           -—max_points_per_cell` | `                                              -x` | `                                         integer` | `                      0 - 10 (worley noise only)` | `                                               4` |


