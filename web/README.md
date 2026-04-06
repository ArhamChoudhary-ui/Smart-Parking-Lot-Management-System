# Web Frontend

Vite-based frontend for the Smart Parking UI.

## Local Development

1. Install dependencies:
   - `npm install`
2. Start development server:
   - `npm run dev`
3. Build production bundle:
   - `npm run build`
4. Preview production build locally:
   - `npm run preview`

## Deploy From Git (Vercel)

When importing this repository in Vercel:

1. Set **Root Directory** to `web`.
2. Build command: `npm run build`
3. Output directory: `dist`

`vercel.json` already includes SPA rewrites to `index.html`.
