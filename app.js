fetch("wallpapers.json")
  .then(response => response.json())
  .then(data => {
    const number = Math.floor(Math.random() * data.count) + 1;

    const filename =
      `wallpaper_${String(number).padStart(3, "0")}.jpg`;

    document.getElementById("wallpaper").src =
      `wallpapers/${filename}`;
  });
