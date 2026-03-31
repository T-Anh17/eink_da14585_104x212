function ImageWorkspace(props: ImageWorkspaceProps) {
  const isVi = props.locale === "vi";
  const [isDragOver, setIsDragOver] = React.useState(false);
  const ditheringHint = (() => {
    if (props.imageParams.dithering === "ordered") {
      return isVi
        ? "Ordered: tạo mẫu hạt đều, xử lý nhanh và ổn định cho ảnh đồ họa."
        : "Ordered: creates an even dot pattern, fast and stable for graphic-style images.";
    }

    if (props.imageParams.dithering === "floyd-steinberg") {
      return isVi
        ? "Floyd-Steinberg: phân tán sai số để ảnh mềm và tự nhiên hơn, hợp với ảnh chụp."
        : "Floyd-Steinberg: diffuses error for softer, more natural-looking results, good for photos.";
    }

    return isVi
      ? "Không dùng: giữ ngưỡng đen trắng cơ bản, phù hợp khi muốn ảnh sắc và ít nhiễu."
      : "None: keeps a basic black-and-white threshold, useful when you want a sharper and cleaner result.";
  })();

  const handleDrop = (event: React.DragEvent<HTMLLabelElement>) => {
    event.preventDefault();
    setIsDragOver(false);
    const file = event.dataTransfer.files?.[0];
    if (file) {
      props.onFileDrop(file);
    }
  };

  return (
    <section className="panel-card image-panel-pro panel-span-two">
      <div className="panel-head">
        <div>
          <span className="eyebrow">{isVi ? "Hình ảnh" : "Image studio"}</span>
          <h2>{isVi ? "Xử lý và nạp nội dung E-Ink" : "Process and deploy E-Ink content"}</h2>
        </div>
      </div>

      <div className="image-studio-grid">
        <div className="image-studio-left">
          <label
            className={isDragOver ? "dropzone-pro is-drag-over" : "dropzone-pro"}
            onDragOver={(event) => {
              event.preventDefault();
              setIsDragOver(true);
            }}
            onDragLeave={() => setIsDragOver(false)}
            onDrop={handleDrop}
          >
            <span className="dropzone-icon" aria-hidden="true">
              <svg viewBox="0 0 24 24">
                <path d="M12 16V8" />
                <path d="M8.8 11.2 12 8l3.2 3.2" />
                <path d="M6 17.5h12" />
                <path d="M7.5 19.5h9" />
              </svg>
            </span>
            <span className="eyebrow">{isVi ? "Kéo thả ảnh" : "Drag and drop"}</span>
            <strong>{isVi ? "Thả hình ảnh vào đây hoặc bấm để chọn file" : "Drop an image here or click to choose a file"}</strong>
            <small>
              {props.imageFile
                ? props.imageFile.name
                : isVi
                  ? "Hỗ trợ ảnh dùng để chuyển sang màn hình E-Ink"
                  : "Supports images that will be converted for the E-Ink display"}
            </small>
            <input type="file" accept="image/*" onChange={props.onFileChange} />
          </label>

          <div className="image-controls-head">
            <div>
              <h3>{isVi ? "Tinh chỉnh hình ảnh" : "Image adjustments"}</h3>
              <p>{isVi ? "Điều chỉnh nhanh trước khi tải lên thiết bị." : "Quick adjustments before uploading to the device."}</p>
            </div>
            <button type="button" className="icon-button reset-button" onClick={props.onResetImageParams}>
              ↺
            </button>
          </div>

          <div className="image-tuning-card image-tuning-plain">
            <div className="setting-group">
              <label htmlFor="brightness">{isVi ? "Độ sáng" : "Brightness"}: {props.imageParams.brightness}%</label>
              <input
                id="brightness"
                type="range"
                min="0"
                max="200"
                value={props.imageParams.brightness}
                onChange={(event) => props.onImageParamChange("brightness", Number(event.target.value))}
              />
            </div>
            <div className="setting-group">
              <label htmlFor="contrast">{isVi ? "Tương phản" : "Contrast"}: {props.imageParams.contrast}%</label>
              <input
                id="contrast"
                type="range"
                min="0"
                max="200"
                value={props.imageParams.contrast}
                onChange={(event) => props.onImageParamChange("contrast", Number(event.target.value))}
              />
            </div>
            <div className="setting-group compact">
              <label htmlFor="dithering">{isVi ? "Kiểu dithering" : "Dithering type"}</label>
              <select
                id="dithering"
                value={props.imageParams.dithering}
                onChange={(event) => props.onImageParamChange("dithering", event.target.value as DitherType)}
              >
                <option value="none">{isVi ? "Không dùng" : "None"}</option>
                <option value="ordered">Ordered</option>
                <option value="floyd-steinberg">Floyd-Steinberg</option>
              </select>
              <small className="setting-hint">{ditheringHint}</small>
            </div>
            <div className="setting-group">
              <label htmlFor="ditherIntensity">
                {isVi ? "Cường độ dithering" : "Dithering strength"}: {props.imageParams.ditherIntensity.toFixed(1)}
              </label>
              <input
                id="ditherIntensity"
                type="range"
                min="0"
                max="2"
                step="0.1"
                value={props.imageParams.ditherIntensity}
                onChange={(event) => props.onImageParamChange("ditherIntensity", Number(event.target.value))}
              />
            </div>
          </div>

          <button
            className="primary-button upload-hero-button"
            disabled={!props.connected || !props.imageFile}
            onClick={props.onSendAll}
          >
            {isVi ? "Xử lý và tải ảnh lên thiết bị" : "Process and upload to device"}
          </button>
        </div>

        <div className="image-studio-right">
          <div className="preview-stage preview-stage-large">
            <div className="preview-head">
              <div>
                <h3>{isVi ? "Xem trước 212x104" : "212x104 preview"}</h3>
                <span>{isVi ? "Ảnh hiển thị theo chuẩn đơn sắc E-Ink" : "Monochrome E-Ink rendering"}</span>
              </div>
            </div>
            <canvas
              ref={props.previewRef}
              width={DISPLAY_WIDTH}
              height={DISPLAY_HEIGHT}
              className="preview-canvas"
            />
          </div>
        </div>
      </div>

      <div className="progress-shell">
        <div className="progress-labels">
          <span>{isVi ? "Tiến trình tải lên" : "Upload progress"}</span>
          <strong>{props.progress}%</strong>
        </div>
        <div id="progress">
          <div id="progress-bar" style={{ width: `${props.progress}%` }} />
        </div>
      </div>
    </section>
  );
}
