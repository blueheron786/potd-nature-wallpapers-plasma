# Wallpaper and Manifest Contract

This document describes the contract between the KDEPRECATED: Manifest-based approach
=====================================

**Note**: The current implementation uses a hardcoded wallpaper count rather than fetching a manifest. This document describes both the legacy manifest-based approach (for reference) and the current hardcoded approach.

## Legacy Manifest-Based Approach (Reference)

This section describes the intended manifest format that was considered but not implemented in the current version.

### wallpapers.json

The manifest file should be located at:
```
https://blueheron786.github.io/potd-nature-wallpapers-plasma/wallpapers.json
```

#### Format

The manifest must be a valid JSON object with the following structure:

```json
{
  "count": 365,
  "epoch": "2026-01-01",
  "wallpapers": [
    {
      "filename": "wallpaper_001.jpg",
      "attribution": {
        "photographer": "Jane Doe",
        "source_url": "https://unsplash.com/photos/example",
        "photographer_url": "https://unsplash.com/@janedoe"
      }
    },
    {
      "filename": "wallpaper_002.jpg",
      "attribution": {
        "photographer": "John Smith",
        "source_url": "https://unsplash.com/photos/example2",
        "photographer_url": "https://unsplash.com/@johnsmith"
      }
    }
    // ... more wallpaper entries
  ]
}
```

#### Field Descriptions

- **count** (integer, required): Total number of wallpapers available. Must be a positive integer.
- **epoch** (string, optional, default: "2026-01-01": The start date for the wallpaper sequence, in ISO 8601 format (YYYY-MM-DD).
- **wallpapers** (array, optional): Array of wallpaper objects for attribution purposes. If present, must have at least 'count' entries.

#### Validation Rules

1. The JSON document must be valid and parseable
2. The root object must be a JSON object (not array, string, etc.)
3. The "count" field must be present and convertible to an integer
4. The count must be greater than 0
5. The count should be reasonable (< 10,000 to prevent excessive resource usage)
6. If "epoch" is present, it must be a valid date in YYYY-MM-DD format
7. If "wallpapers" array is present, its length should be >= count

#### Error Handling

If the manifest fails validation:
- Log a warning with the specific validation failure
- Emit the provider's error signal
- Do not attempt to download any wallpaper images

## Current Implementation (Hardcoded Count)

The current implementation does not fetch or parse a manifest. Instead, it uses:

### Constants

- **Base URL**: `https://blueheron786.github.io/potd-nature-wallpapers-plasma/`
- **Epoch Date**: 2026-01-01 (hardcoded)
- **Wallpaper Count**: `DefaultWallpaperCount` (currently 365, hardcoded in source)
- **Wallpaper URL Pattern**: `wallpapers/wallpaper_{number:03d}.jpg`

### Wallpaper Selection Algorithm

1. Calculate days since epoch: `days = epoch.daysTo(requestedDate)`
2. Compute wallpaper index: `index = ((days % count) + count) % count`
3. Wallpaper number: `number = index + 1` (1-based indexing)
4. Format filename: `wallpaper_{number:03d}.jpg`
5. Construct URL: `baseUrl + "wallpapers/" + filename`

### Properties

- **Deterministic**: Same date always produces same wallpaper
- **Wraparound**: Sequences repeat after `count` days
- **Before Epoch**: Dates before 2026-01-01 wrap to end of sequence
- **Leap Year Aware**: Uses QDate for accurate day counting
- **No Network Dependency for Logic**: Date-to-number calculation works offline

### Error Conditions

The current implementation will error if:
- `DefaultWallpaperCount <= 0` (configuration error)
- Failed to download manifest (network or HTTP error)
- Manifest response is empty
- Failed to parse manifest as JSON
- Manifest is not a JSON object
- Missing or invalid "count" field in manifest
- Count <= 0 or count > 10000
- Invalid requested date from arguments
- Failed to download wallpaper image
- Wallpaper image data is empty
- Failed to decode image data

## Future Manifest Implementation

To transition to a manifest-based approach in the future:

1. Restore manifest fetching in `NatureProvider` constructor
2. Parse and validate manifest per the contract above
3. Extract count, epoch, and optional attribution data
4. Use manifest-provided values instead of hardcoded constants
5. Maintain backward compatibility by providing sensible defaults
6. Cache manifest data to avoid repeated network requests
7. Implement manifest expiration/refetching strategy

## Deployment Requirements

For the provider to function correctly:

1. Wallpaper images must be available at:
   `https://blueheron786.github.io/potd-nature-wallpapers-plasma/wallpapers/wallpaper_{NNN}.jpg`
   where NNN is a 3-digit number from 001 to count

2. All images must be valid JPEG files

3. Images should be reasonably sized for desktop wallpapers (recommended: minimum 1920x1080)

4. The GitHub Pages site must be publicly accessible over HTTPS

5. If implementing attribution display, the manifest must be deployed and accessible

## Backward Compatibility

Any changes to this contract should maintain backward compatibility:

- Provider identifier ("nature") must remain unchanged
- Basic date-to-wallpaper mapping algorithm should remain the same
- Fallback behavior should be graceful when manifest is unavailable
- Existing wallpaper filename convention (wallpaper_XXX.jpg) should be preserved