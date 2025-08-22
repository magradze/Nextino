import type {ReactNode} from 'react';
import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

type FeatureItem = {
  title: string;
  Svg: React.ComponentType<React.ComponentProps<'svg'>>;
  description: ReactNode;
};

const FeatureList: FeatureItem[] = [
  {
    title: '🚀 Modular Architecture',
    Svg: require('@site/static/img/undraw_building_blocks.svg').default,
    description: (
      <>
        Break free from messy <code>.ino</code> files.  
        Nextino introduces a clean, <b>module-based structure</b>  
        for Arduino, ESP32, STM32, and beyond.
      </>
    ),
  },
  {
    title: '⚡ Arduino-Compatible',
    Svg: require('@site/static/img/undraw_coding.svg').default,
    description: (
      <>
        Keep using the <b>Arduino libraries</b> you already love.  
        Nextino adds structure, logging, and scalability  
        — without breaking compatibility.
      </>
    ),
  },
  {
    title: '🛠️ Scalable & Production-Ready',
    Svg: require('@site/static/img/undraw_neon_world.svg').default,
    description: (
      <>
        Built-in <b>non-blocking scheduler</b>,  
        <b>config-driven modules</b>, and  
        <b>professional logging</b> — perfect for real IoT projects.
      </>
    ),
  },
];

function Feature({title, Svg, description}: FeatureItem) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        <Svg className={styles.featureSvg} role="img" />
      </div>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures(): ReactNode {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
    </div>
    </section>
  );
}
