const express = require("express");
const path = require("path");
const app = express();
const PORT = 3030;

// Указываем папку с статическими файлами (например, index.html)
app.use(express.static(path.join(__dirname)));

app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "index.html"));
});

// Запуск сервера
app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});
