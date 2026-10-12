# Enclosure variants

[中文](../zh-CN/enclosure.md) · [Home](../../README.md)


## Included geometry

| Files | Geometry interpretation | Physical status |
| --- | --- | --- |
| `盒盖.stp` + `后盖.stp` | Main box with separate flat rear plate and eight projecting fastening posts: screw-fastened approach | The screw-fastened approach was printed, as confirmed by the maintainer |
| `盒盖A.stp` + `后盖A.stp` | Main box with separate U-shaped outer frame: sliding-frame approach | Not physically printed |

These pairings are inferred from reading and rendering the actual STEP geometry, not from the A suffix alone. The original Chinese filenames are retained. Model geometry is included as design source; exact fasteners, print settings, clearances and fit need to be checked on your own build. STEP files were imported successfully as one solid each.

### Screw-fastened approach

![Main box](../assets/enclosure-lid.png)

![Rear plate with posts](../assets/enclosure-back.png)

### Sliding-frame approach

![A main box](../assets/enclosure-lid-a.png)

![A U-shaped frame](../assets/enclosure-back-a.png)

The images are model renders, not photographs of a printed assembly. Import the STEP files into CAD or a compatible slicer; export your own STL/3MF if needed. Avoid rescaling the model to fit a printer without checking module and connector clearances. Weather sealing and material temperature performance are not established by these models.
