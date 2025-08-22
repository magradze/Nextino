import React, { JSX } from 'react';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';
import HomepageFeatures from '@site/src/components/HomepageFeatures';

export default function Home(): JSX.Element {
  return (
    <Layout
      title="Nextino Framework"
      description="The Next Step for Arduino — A structured, scalable, and modern C++ framework for Arduino, ESP32, STM32, and beyond."
    >
      <header className="hero hero--primary">
        <div className="container text--center">
          <img src="img/NEXT.ino.png" alt="Nextino Logo" width="180" />
          <h1 className="hero__title">Nextino Framework (v0.1.0)</h1>
          <p className="hero__subtitle">
            The Next Step for Arduino — structured, scalable, modern.
          </p>
          <div className="buttons">
            <Link className="button button--secondary button--lg margin--sm" to="/docs/intro">
              🚀 Get Started
            </Link>
            <Link className="button button--outline button--lg margin--sm" to="https://github.com/magradze/Nextino">
              ⭐ GitHub
            </Link>
          </div>
        </div>
      </header>

      <main>
        <HomepageFeatures />

        <section className="margin-vert--xl container">
          <h2 className="text--center">🎉 The Story Behind Nextino</h2>
          <p>
            Nextino’s first public release coincides with a very special day — <b>my son’s birthday</b>.  
            Just like him, Nextino is <b>just starting its journey</b>, full of potential and possibilities.  
            Two birthdays on the same day: one of my son, one of my creation. Both equally precious. 🎈
          </p>
        </section>
      </main>
    </Layout>
  );
}
