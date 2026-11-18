# 余帆 · 嵌入式硬件工程师作品集

纯静态个人作品集。深色主题，单一强调色，无外部依赖，不加载任何 CDN。

**线上地址：** https://yohoda.github.io/yufan-portfolio/

**备用地址：** https://inland-skunk-d4germ4o05c8dd921-1535687828.tcloudbaseapp.com/ （腾讯云 CloudBase，国内更快，但有「测试域名」提示页，需点一次继续访问）

## 内容

| 页面 | 说明 |
|:--|:--|
| `index.html` | 首页：Hero、四个项目卡、技能、联系方式 |
| `projects/bldc-driver.html` | 无刷电机驱动板（STM32G474，FOC / 六步换相） |
| `projects/usb-hub.html` | USB3.0 拓展坞（VL812，1 进 4 出） |
| `projects/pocket-pet.html` | 掌上宠物（STM32F103 + 离线语音模块） |
| `projects/balance-car.html` | 两轮循迹平衡小车（双环 PID + MPU6050） |
| `404.html` | 错误页 |

## 技术

- 纯 HTML / CSS / 原生 JS
- 字体走系统字体栈，国内无需等外部资源
- 图片为 WebP，单张控制在 120 KB 以内
- 响应式：820px（技能区单列）、720px（项目卡单列）
- 尊重 `prefers-reduced-motion`

## 本地预览

双击 `index.html` 即可，无需构建、无需起服务器。

## 部署

托管在腾讯云 CloudBase 静态网站托管，索引文档 `index.html`，错误文档 `404.html`。

上传通过 DSH 的 CloudBase MCP 完成，文件直接覆盖到托管根目录。

## 素材说明

页面中标注「待补」的占位块是留待替换的图片位。替换时把 `<div class="media">` 内的
`<span>` 换成 `<img>`，并给该 `div` 加上 `has-media` 类，占位虚线会自动消失。
