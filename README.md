# noisy

Generate procedural noise from from the command line.

- Supports Perlin and Worley noise
- Can generate 2D and 3D noise
- Seemless tiling in all dimensions
- Very easy to use

## Created with noisy
![screenshot](https://github.com/aaron9000/noisy/blob/master/media/screenshot.png)
--


## Installation
```
npm install -g noisy
```


## Recipes

##### See visual diff of two files
```csvutil -a sample_a.csv -b sample_b.csv```

##### Preview the first 30 rows
```csvutil -a sample_a.csv -r 30```

##### Preview all rows
```csvutil -a sample_a.csv -r 0```

##### Render a markdown snippet
```csvutil -a sample_a.csv -o md```

##### Save diff to a file
```csvutil -a sample_a.csv -b sample_b.csv -o csv > file.csv```


## Argument Reference
|                                               long |                                              short |                                               type |                                        description |                                            default |
|----------------------------------------------------|----------------------------------------------------|----------------------------------------------------|----------------------------------------------------|----------------------------------------------------|
| `                                        --path-a` | `                                              -a` | `                                       file path` | `                                 source csv path` | `                                            null` |
| `                                        --path-b` | `                                              -b` | `                                       file path` | `                  comparison csv path (for diff)` | `                                            null` |
| `                                        --output` | `                                              -o` | `                                          string` | `                   output format (md, csv, pretty)` | `                                          pretty` |
| `                                          --rows` | `                                              -r` | `                                         integer` | `            max rows to output (0 for unlimited)` | `                                              10` |
| `                                         --width` | `                                              -w` | `                                         integer` | `      max width for pretty & md columns (5 - 50)` | `                                              16` |

