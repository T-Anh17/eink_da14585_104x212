import { createServer } from "node:http";
import { readFile } from "node:fs/promises";
import { existsSync, watch } from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";
import { spawn } from "node:child_process";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const rootDir = path.resolve(__dirname, "..");
const host = "127.0.0.1";
const port = Number(process.env.PORT || 3000);
const clients = new Set();
let browserOpened = false;

const mimeTypes = {
  ".html": "text/html; charset=utf-8",
  ".js": "application/javascript; charset=utf-8",
  ".css": "text/css; charset=utf-8",
  ".json": "application/json; charset=utf-8",
  ".map": "application/json; charset=utf-8",
  ".png": "image/png",
  ".jpg": "image/jpeg",
  ".jpeg": "image/jpeg",
  ".gif": "image/gif",
  ".svg": "image/svg+xml",
  ".ico": "image/x-icon",
};

function sendReload() {
  for (const response of clients) {
    response.write("data: reload\n\n");
  }
}

function openBrowser(url) {
  if (browserOpened || process.env.NO_OPEN === "1") {
    return;
  }

  browserOpened = true;

  if (process.platform === "win32") {
    spawn("cmd", ["/c", "start", "", url], {
      cwd: rootDir,
      detached: true,
      stdio: "ignore",
    }).unref();
    return;
  }

  if (process.platform === "darwin") {
    spawn("open", [url], {
      cwd: rootDir,
      detached: true,
      stdio: "ignore",
    }).unref();
    return;
  }

  spawn("xdg-open", [url], {
    cwd: rootDir,
    detached: true,
    stdio: "ignore",
  }).unref();
}

function injectDevClient(html) {
  const devClient = `
<script>
  if ("EventSource" in window) {
    const devEvents = new EventSource("/__dev_events");
    devEvents.onmessage = (event) => {
      if (event.data === "reload") {
        window.location.reload();
      }
    };
  }
</script>`;
  return html.includes("</body>") ? html.replace("</body>", `${devClient}\n</body>`) : `${html}\n${devClient}`;
}

function resolveFilePath(urlPath) {
  const pathname = decodeURIComponent(urlPath.split("?")[0]);
  const requested = pathname === "/" ? "/index.html" : pathname;
  const filePath = path.resolve(rootDir, `.${requested}`);
  if (!filePath.startsWith(rootDir)) {
    return null;
  }
  return filePath;
}

const server = createServer(async (request, response) => {
  const urlPath = request.url || "/";

  if (urlPath === "/__dev_events") {
    response.writeHead(200, {
      "Content-Type": "text/event-stream",
      "Cache-Control": "no-cache",
      Connection: "keep-alive",
      "Access-Control-Allow-Origin": "*",
    });
    response.write("\n");
    clients.add(response);
    request.on("close", () => {
      clients.delete(response);
    });
    return;
  }

  const filePath = resolveFilePath(urlPath);
  if (!filePath || !existsSync(filePath)) {
    response.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
    response.end("Not found");
    return;
  }

  try {
    const ext = path.extname(filePath).toLowerCase();
    const mimeType = mimeTypes[ext] || "application/octet-stream";
    let content = await readFile(filePath);

    if (ext === ".html") {
      content = Buffer.from(injectDevClient(content.toString("utf8")), "utf8");
    }

    response.writeHead(200, { "Content-Type": mimeType, "Cache-Control": "no-store" });
    response.end(content);
  } catch (error) {
    response.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
    response.end(`Server error: ${error instanceof Error ? error.message : String(error)}`);
  }
});

const compiler = spawn(
  process.execPath,
  ["./node_modules/typescript/bin/tsc", "-p", "tsconfig.json", "-w", "--preserveWatchOutput"],
  { cwd: rootDir, stdio: "inherit" },
);

watch(rootDir, { recursive: true }, (_, filename) => {
  if (!filename) {
    return;
  }
  const normalized = filename.replace(/\\/g, "/");
  if (normalized === "index.html" || normalized === "style.css" || normalized.startsWith("dist/")) {
    sendReload();
  }
});

function shutdown(code = 0) {
  for (const response of clients) {
    response.end();
  }
  clients.clear();
  server.close(() => {
    if (!compiler.killed) {
      compiler.kill();
    }
    process.exit(code);
  });
}

process.on("SIGINT", () => void shutdown(0));
process.on("SIGTERM", () => void shutdown(0));

server.listen(port, host, () => {
  const url = `http://${host}:${port}`;
  console.log(`Dev server running at ${url}`);
  openBrowser(url);
});
